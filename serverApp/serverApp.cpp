#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <interface.h>
#include <niDAQWebInterface.h>
#include<warehouseWebServer.h>

#include <MQTT_client.h>

#include<localControl.h>

int main()
{
    printf("Welcome to Intelligent Supervision\n");
    printf("press key");

    configure_simulator_server();
    start_mg_server();
    initializeHardwarePorts();
    startMQTTActuatorsOperation();
    startMQTTSensorsOperation();

    int keyboard = 0;

    showLocalMenu();
    while (keyboard != 27) {
        if (_kbhit()) {
            keyboard = _getch();
            executeLocalControl(keyboard);
        }
        else {
            keyboard = 0;
        }
        Sleep(1);
    }

    stop_mg_server();
    return 0;
}
