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

void onMqttActuatorsConnectFailure(void* context, MQTTAsync_failureData* response);
void onMqttActuatorsConnectSuccess(void* context, MQTTAsync_successData* response);

void startMQTTActuatorsOperation();
void startMQTTSensorsOperation();

void onMqttActuatorsConnectionLost(void* context, char* cause);
int onMqttActuatorsMessageArrived(void* context, char* topicName, int messageLen, MQTTAsync_message* message);

void mqttMonitorXAxis(MqttClientManager& mqttClientManager);
void mqttMonitorYAxis(MqttClientManager& mqttClientManager);
void mqttMonitorZAxis(MqttClientManager& mqttClientManager);
void mqttMonitorLeftStation(MqttClientManager& mqttClientManager);
void mqttMonitorRightStation(MqttClientManager& mqttClientManager);
void mqttMonitorCage(MqttClientManager& mqttClientManager);
// other headings declarations later on.
