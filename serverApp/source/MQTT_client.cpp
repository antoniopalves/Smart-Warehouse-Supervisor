#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include<vector>
#include<string>
#include<thread>

#include <interface.h>
#include<warehouseWebServer.h>
#include<localControl.h>

#include <MqttClientManager.h>

#include <MQTT_client.h>

#include<vector>
#include<string>

#include <nlohmann/json.hpp>

#include<chrono>
#include<thread>
bool MqttSystemIsRunning = true;

using json = nlohmann::json;


extern bool MqttSystemIsRunning;

#define MQTT_BROKER_URL  (const char *) "tcp://localhost:1883"

void onMqttActuatorsConnectFailure(void* context, MQTTAsync_failureData* response)
{
    printf("Mqtt Actuators connect FAILURE, rc = %d\n", response->code);

}

void onMqttActuatorsConnectSuccess(void* context, MQTTAsync_successData* response)
{
    printf("Mqtt Actuators connect SUCCESS\n");
    MqttClientManager* client = (MqttClientManager*)context;
    std::vector<std::string> topics = { "menu_keyboard", "actuator" };
    client->subscribe(topics, QOS_1, onMqttActuatorsConnectionLost, onMqttActuatorsMessageArrived, NULL);
}


void startMQTTActuatorsOperation() {
    // the static specifier below is to avoid the variable 
    // being declared in the stack, which is volatile. 
    static MqttClientManager mqttActuators;
    // it can be any unique identifier
    mqttActuators.create("actuators_client_id_1", MQTT_BROKER_URL);
    mqttActuators.connect(onMqttActuatorsConnectSuccess, onMqttActuatorsConnectFailure);
}

void onMqttActuatorsConnectionLost(void* context, char* cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int onMqttActuatorsMessageArrived(void* context, char* topicName, int messageLen, MQTTAsync_message* message)
{
    char* payload = (char*)message->payload;

    if (strcmp(topicName, "menu_keyboard") == 0) {
        // mosquitto_pub -h localhost -p 1883 -t "menu_keyboard" -m "a"
        int key_command = payload[0];  // consider only the first character
        executeLocalControl(key_command);
    }
    else if (strcmp(topicName, "actuator") == 0) {
        try {
            json jsonMessage = json::parse(payload);

            std::string name = jsonMessage["name"];
            std::string value = jsonMessage["value"];

            printf("\nParsed JSON - name: %s, value: %s", name.c_str(), value.c_str());

            // Control logic for the actuator motor_x
            if (name == "motor_x") { // the class string has got operator ==
                int direction = std::stoi(value);  // Convert value to an integer
                if (direction == 0) { stopX(); } // if 
                if (direction == 1) { moveXRight(); } // else if
                if (direction == -1) { moveXLeft(); } // else if
            }
            else if (name == "motor_y") { // the class string has got operator ==
                int direction = std::stoi(value);  // Convert value to an integer
                if (direction == 0) { stopY(); } // if 
                if (direction == -1) { moveYInside(); } // else if
                if (direction == 1) { moveYOutside(); } // else if
            }
            else if (name == "motor_z") { // the class string has got operator ==
                int direction = std::stoi(value);  // Convert value to an integer
                if (direction == 0) { stopZ(); } // if 
                if (direction == -1) { moveZDown(); } // else if
                if (direction == 1) { moveZUp(); } // else if
            }
            else if (name == "motor_ls") { // the class string has got operator ==
                int direction = std::stoi(value);  // Convert value to an integer
                if (direction == 0) { stopLeftStation(); } // if 
                if (direction == 1) { moveLeftStationInside(); } // else if
                if (direction == -1) { moveLeftStationOutside(); } // else if
            }
            else if (name == "motor_rs") { // the class string has got operator ==
                int direction = std::stoi(value);  // Convert value to an integer
                if (direction == 0) { stopRightStation(); } // if 
                if (direction == 1) { moveRightStationInside(); } // else if
                if (direction == -1) { moveRightStationOutside(); } // else if
            }
            // Add more actuator controls here as needed...
        }
        catch (json::exception& e) {
            // Catch any errors during JSON parsing
            printf("\nError parsing JSON: %s", e.what());
        }
    }
    return 1;
}

void startMQTTSensorsOperation() {
    std::thread t([]() {
        printf("\nmonitoring sensors started...");
        MqttClientManager mqttMonitoring;
        mqttMonitoring.create("sensors_client_id_1", MQTT_BROKER_URL);
        mqttMonitoring.connect(NULL, NULL);

        // here, instead of doing this code after success connection handle 
        // we wait till successful connection
        // the program does not freeze here, because this is a new thread.
        printf("\nWaiting for mqtt monitoring_client_1 connection...");
        while (mqttMonitoring.isConnected() == false) {
            putchar('.');
            Sleep(1000);
        }
        while (MqttSystemIsRunning) {
            mqttMonitorXAxis(mqttMonitoring);
            mqttMonitorYAxis(mqttMonitoring);
            mqttMonitorZAxis(mqttMonitoring);
            mqttMonitorLeftStation(mqttMonitoring);
            mqttMonitorRightStation(mqttMonitoring);
            mqttMonitorCage(mqttMonitoring);
        }
        mqttMonitoring.disconnect();
        printf("\nmonitoring finishing...");
        });
    t.detach();
}

void mqttMonitorXAxis(MqttClientManager& mqttClientManager) {
    static auto lastTime = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    static int previous_x_position = -1, previous_x_moving = -9999;
    int xPosition = getXPosition();
    int xMoving = getXDirection();

    char message[128] = "";
    // publish the x position info when: the position has changed, the movement has changed, and every 10 seconds.
    if ((xPosition != previous_x_position) || (xMoving != previous_x_moving) || (duration > 10000)) {
        sprintf(message, "{\"name\": \"x_is_at\", \"value\": \"%d\"}", xPosition);
        previous_x_position = xPosition;
        lastTime = currentTime;
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        sprintf(message, "{\"name\": \"x_direction\", \"value\": \"%d\"}", xMoving);
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        previous_x_moving = xMoving;
    }
}

void mqttMonitorYAxis(MqttClientManager& mqttClientManager) {
    static auto lastTime = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    static int previous_y_position = -1, previous_y_moving = -9999;
    int yPosition = getYPosition();
    int yMoving = getYDirection();

    char message[128] = "";
    // publish the x position info when: the position has changed, the movement has changed, and every 10 seconds.
    if ((yPosition != previous_y_position) || (yMoving != previous_y_moving) || (duration > 10000)) {
        sprintf(message, "{\"name\": \"y_is_at\", \"value\": \"%d\"}", yPosition);
        previous_y_position = yPosition;
        lastTime = currentTime;
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        sprintf(message, "{\"name\": \"x_direction\", \"value\": \"%d\"}", yMoving);
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        previous_y_moving = yMoving;
    }
}

void mqttMonitorZAxis(MqttClientManager& mqttClientManager) {
    static auto lastTime = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    static int previous_z_position = -1, previous_z_moving = -9999;
    int zPosition = getZPosition();
    int zMoving = getZDirection();

    char message[128] = "";
    // publish the x position info when: the position has changed, the movement has changed, and every 10 seconds.
    if ((zPosition != previous_z_position) || (zMoving != previous_z_moving) || (duration > 10000)) {
        sprintf(message, "{\"name\": \"z_is_at\", \"value\": \"%d\"}", zPosition);
        previous_z_position = zPosition;
        lastTime = currentTime;
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        sprintf(message, "{\"name\": \"x_direction\", \"value\": \"%d\"}", zMoving);
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        previous_z_moving = zMoving;
    }
}

void mqttMonitorLeftStation(MqttClientManager& mqttClientManager) {
    static auto lastTime = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    static int previous_station_status = -9999, previous_station_has_part = -9999;

    int stationDirection = getLeftStationDirection(); // -1: moving outside, 0: stopped, 1: moving inside
    bool stationHasPart = isPartOnLeftStation(); 

    char message[128] = "";
    // Publish the station status when: direction has changed, part status has changed, or every 10 seconds.
    if ((stationDirection != previous_station_status) || (stationHasPart != previous_station_has_part) || (duration > 10000)) {
        sprintf(message, "{\"name\": \"left_station_direction\", \"value\": \"%d\"}", stationDirection);
        previous_station_status = stationDirection;
        lastTime = currentTime;
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);

        sprintf(message, "{\"name\": \"left_station_has_part\", \"value\": \"%d\"}", stationHasPart);
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        previous_station_has_part = stationHasPart;
    }
}

void mqttMonitorRightStation(MqttClientManager& mqttClientManager) {
    static auto lastTime = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    static int previous_station_status = -9999, previous_station_has_part = -9999;

    int stationDirection = getRightStationDirection(); // -1: moving outside, 0: stopped, 1: moving inside
    bool stationHasPart = isPartOnRightStation(); // Assuming this function returns true if there's a part on the right station

    char message[128] = "";
    // Publish the station status when: direction has changed, part status has changed, or every 10 seconds.
    if ((stationDirection != previous_station_status) || (stationHasPart != previous_station_has_part) || (duration > 10000)) {
        sprintf(message, "{\"name\": \"right_station_direction\", \"value\": \"%d\"}", stationDirection);
        previous_station_status = stationDirection;
        lastTime = currentTime;
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);

        sprintf(message, "{\"name\": \"right_station_has_part\", \"value\": \"%d\"}", stationHasPart);
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
        previous_station_has_part = stationHasPart;
    }
}
void mqttMonitorCage(MqttClientManager& mqttClientManager) {
    static auto lastTime = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    static bool previous_cage_has_part = false;

    bool cageHasPart = isPartInCage(); // Assuming this function returns true if a part is in the cage

    char message[128] = "";
    // Publish the cage status when the part presence has changed or every 10 seconds.
    if ((cageHasPart != previous_cage_has_part) || (duration > 10000)) {
        sprintf(message, "{\"name\": \"cage_has_part\", \"value\": \"%d\"}", cageHasPart);
        previous_cage_has_part = cageHasPart;
        lastTime = currentTime;
        mqttClientManager.publish(message, "sensor", QOS_1, NULL, NULL);
    }
}


