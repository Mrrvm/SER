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
void dist_handler() {
    printf("Created distance thread");
    int distance;
    while(1) {
        distance = distanceSensor();
        printf("Reading distance %d\n", distance);
        if(distance < DEFAULT_DIST) {
            //can_mutex.lock();
            printf("Sending distance %d\n", distance);
            send(CODE_FRONT_DISTANCE, distance);
            //can_mutex.unlock();
        }
        wait_ms(1000);   
    }
}


int main() {

    Thread t_msg_handler;
    Thread t_dist_handler;
    int ldr, new_ldr;
    t_dist_handler.start(dist_handler);
    t_msg_handler.start(msg_handler);
    
    while(1) {
        ldr = LDRSensor();
        printf("Reading ldr %d\n", ldr);
        if(ldr < DEFAULT_LIGHT){
            //can_mutex.lock();
            printf("Sending ldr %d\n", ldr);
            send(CODE_LDR, ldr);
            //can_mutex.unlock();
            LDRHandler(1);
        }
        wait_ms(1000);     
    }
}










