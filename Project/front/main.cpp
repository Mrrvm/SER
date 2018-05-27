#include "interface.h"

int lum = LIGHTS_OFF;

void distInterrupt(void const *aux) {
    int distance;
    distance = distanceSensor();
    if(distance < DEFAULT_DIST) {
        send(CODE_FRONT_DISTANCE, distance);
    }
}

void msgInterrupt(void const *aux) {
    receive();
}

void LDRInterrupt(void const *aux) {
    int ldr;
    ldr = LDRSensor(); 
    if(lum != LIGHTS_ON && ldr > DEFAULT_LIGHT) {
        lum = LIGHTS_ON;
        send(CODE_LDR, lum);
        LDRHandler(lum);
    }
    else if(lum != LIGHTS_OFF && ldr <= DEFAULT_LIGHT) {
        lum = LIGHTS_OFF;
        send(CODE_LDR, lum);
        LDRHandler(lum);
    }
}

int main() {
    
    PHY_PowerDown();
    RtosTimer m_ticker(msgInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer d_ticker(distInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer l_ticker(LDRInterrupt, osTimerPeriodic, (void *)0);
    m_ticker.start(SLEEP_CHECK_TIME);
    
    if(state == SLEEP) {
        sleep();
    }
    
    while(1) {
        if(state == SLEEP) {
            m_ticker.stop();
            m_ticker.start(SLEEP_CHECK_TIME);
            d_ticker.stop();
            l_ticker.stop();
            sleep();
        }
        else if(state == ON) {
            m_ticker.stop();
            m_ticker.start(MSG_CHECK_TIME);
            d_ticker.start(DIST_CHECK_TIME);
            l_ticker.start(LDR_CHECK_TIME);
        }
        wait_ms(STATE_CHECK_TIME);
    }
}
