#include "mbed.h"
#include "LM75B.h"

CAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD);

char counter = 0;


int main() {
    LM75B sensor(p28,p27);
    Serial pc(USBTX,USBRX);
    //char * msg;
    
    while(1) {
        printf("send()\n");
        if(can2.write(CANMessage(1500, &counter, 1))) {
            printf("wloop()\n");
            counter++;
            printf("Message sent: %d\n", counter);
        }
        wait(0.2);
    } 
    
    //Try to open the LM75B
    /*
    if (sensor.open()) {
        printf("Device detected!\n");
 
   
        while (1) {
            printf("Here\n");
            sprintf(msg, "Temp = %.3f\n", (float)sensor.temp());
            can2.write(CANMessage(1, msg, sizeof(msg)));
            wait(1.0);
            printf("Sent msg of size %d\n", sizeof(msg));
        }
 
    } else {
        error("Device not detected!\n");
    }*/
}


