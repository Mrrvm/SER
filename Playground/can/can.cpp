#include "mbed.h"

Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
CAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD);
char counter = 0;

void send() {
    printf("send()\n");
    if(can1.write(CANMessage(1337, &counter, 1))) {
        printf("wloop()\n");
        counter++;
        printf("Message sent: %d\n", counter);
    } 
    led1 = !led1;
}

int main() {
    printf("main()\n");
    ticker.attach(&send, 1);
    CANMessage msg;
    while(1) {
        printf("loop()\n");
        if(can2.read(msg)) {
            printf("Message received: %d\n", msg.data[0]);
            led2 = !led2;
        } 
        wait(0.2);
    }
}
