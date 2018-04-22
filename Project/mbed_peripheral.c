#include "mbed.h"

CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
CAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD);

int main() {
    CANMessage msg;
    printf("Entering loop\n");
    while(1) {
        if(can2.read(msg)) 
            printf("Message received: %d\n", msg.data[0]);
        wait(0.1);
    } 
}
