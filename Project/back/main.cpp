#include "interface.h"

int windshields = RAIN_OFF; 

void distInterrupt(void const *aux) {
    int distance;
    distance = distanceSensor();
    if(distance < DEFAULT_DIST) {
        send(CODE_BACK_DISTANCE, distance);
    }
}

void msgInterrupt(void const *aux) {
    receive();
}

void rainInterrupt(void const *aux) {
    int rain;
    rain = rainSensor(); 
    printf("[Rain] %d", rain);
    if(windshields != RAIN_OFF && rain < RAIN_OFF) {
        windshields = RAIN_OFF;
    } 
    else if(windshields != RAIN_L1 && rain < RAIN_L1) {
        windshields = RAIN_L1;
    }
    else if(windshields != RAIN_L2 && rain < RAIN_L2){
        windshields = RAIN_L2;
    }
    else if(windshields != RAIN_L3 && rain >= RAIN_L2) {
        windshields = RAIN_L3;
    }
    send(CODE_RAIN, windshields);
    rainHandler(windshields);
}

int main() {
    
    PHY_PowerDown();
    RtosTimer m_ticker(msgInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer d_ticker(distInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer r_ticker(rainInterrupt, osTimerPeriodic, (void *)0);
    m_ticker.start(SLEEP_CHECK_TIME);
    
    if(state == SLEEP) {
        sleep();
    }
    
    while(1) {
        if(state == SLEEP) {
            m_ticker.stop();
            m_ticker.start(SLEEP_CHECK_TIME);
            d_ticker.stop();
            r_ticker.stop();
            sleep();
        }
        else if(state == ON) {
            m_ticker.stop();
            m_ticker.start(MSG_CHECK_TIME);
            d_ticker.start(DIST_CHECK_TIME);
            r_ticker.start(RAIN_CHECK_TIME);
        }
        wait_ms(STATE_CHECK_TIME);
    }
}
