#include "interface.h"
#include "rtos.h"
Mutex can_mutex;

void msg_handler() {
    printf("Created message thread\n5");
    while(1) {
        
        receive();
        wait_ms(1000);
    }
}
void dist_handler() {
    printf("Created distance thread\n");
    int distance;
    while(1) {
        distance = distanceSensor();
        printf("Reading distance %d\n", distance);
        if(distance < DEFAULT_DIST) {
            //can_mutex.lock();
            printf("Sending distance %d", distance);
            send(CODE_BACK_DISTANCE, distance);
            //can_mutex.unlock();
        }
        wait_ms(1000);   
    }
}


int main() {

    Thread t_msg_handler;
    int rain, new_rain;
    Thread t_dist_handler;
    t_dist_handler.start(dist_handler);
    t_msg_handler.start(msg_handler);

    while(1) {
        rain = rainSensor();
        printf("Reading rain %d\n", rain);
        if(rain > DEFAULT_RAIN){
            //can_mutex.lock();
            send(CODE_RAIN, rain);
            printf("Sending rain %d\n", rain);
            //can_mutex.unlock();
            rainHandler(rain);
        }

        wait_ms(1000); 
    }
}




