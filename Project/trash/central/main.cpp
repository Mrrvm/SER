#include "interface.h"
#include "rtos.h"
Mutex can_mutex;

void msg_handler() {
    printf("Created message thread");
    while(1) {
        receive();
        wait_ms(1000);
    }
}

int main() {
    Thread t_msg_handler;
    int temp, weight;
    t_msg_handler.start(msg_handler);
    while(1) {
        temp = temperatureSensor();
        if(temp != DEFAULT_TEMP) {
            temperatureHandler(DEFAULT_TEMP - temp);
        }
        weight = weightSensor();
        if(weight != DEFAULT_WEIGHT) {
            weightHandler(DEFAULT_WEIGHT - weight);
        }
        wait_ms(1000);
    }
}










