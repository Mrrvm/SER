#include "interface.h"

Mutex m_state;
int state = OFF, state_before = OFF;

void tempInterrupt(void const *aux) {
    int temp;
    temp = temperatureSensor();
    if(temp != DEFAULT_TEMP) {
        temperatureHandler(DEFAULT_TEMP-temp);
    }
}

void msgInterrupt(void const *aux) {
    if(!receive()) {
        LCD_msg *msg;
        sprintf(msg->_string, "[CAN] Cannot read messages");
        LCD_queue.put(msg);
    }
}
    
void weightInterrupt(void const *aux) {
    int weight;
    weight = weightSensor();
    if(weight < DEFAULT_WEIGHT && state == IDLE) {
        m_state.lock();
        state = OFF;
        m_state.unlock();
        weightHandler(weight);
    }
    else if(weight >= DEFAULT_WEIGHT && state == OFF){
        m_state.lock();
        state = IDLE;
        m_state.unlock();
        weightHandler(weight);
    }
}

void potInterrupt(void const *aux) {
    int _pot;
    _pot = readPot();
    if(_pot >= DEFAULT_POT && state == IDLE) {
        m_state.lock();
        state = ON;
        m_state.unlock();
        potHandler(_pot);
    }
    else if(_pot < DEFAULT_POT && state == ON) {
        m_state.lock();
        state = IDLE;
        m_state.unlock();
        potHandler(_pot);
    }
}

void lcdInterrupt(void const *aux) {
    lcd.cls();
    lcd.locate(0,3);
    osEvent evt = LCD_queue.get();
    if (evt.status == osEventMessage) {
        LCD_msg *msg = (LCD_msg *)evt.value.p;
        lcd.printf(msg->_string);
    }
}

int main() {
    
    PHY_PowerDown();
    int slave_state = 0;
    RtosTimer w_ticker(weightInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer p_ticker(potInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer m_ticker(msgInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer lcd_ticker(lcdInterrupt, osTimerPeriodic, (void *)0);
    RtosTimer temp_ticker(tempInterrupt, osTimerPeriodic, (void *)0);
    w_ticker.start(WEIGHT_CHECK_TIME);
    
    while(1) {
        if(state_before == ON && state == IDLE) {
            // Kill all mbeds
            slave_state = 0;
            send(CODE_INTERRUPT, slave_state);
            // Start checking weight
            w_ticker.start(WEIGHT_CHECK_TIME);
            // Stop recieving from CAN
            m_ticker.stop();
            state_before = IDLE;
        }
        if(state_before == OFF && state == IDLE) {
            // Activate lcd ticker
            lcd_ticker.start(LCD_CHECK_TIME);
            // Activate potenciometer ticker
            p_ticker.start(POT_CHECK_TIME);
            // Start checking temperature
            temp_ticker.start(TEMP_CHECK_TIME);
            state_before = IDLE;
        }
        if(state_before == IDLE && state == ON) {
            // Wake up mbeds
            slave_state = 1;
            send(CODE_INTERRUPT, slave_state);
            // Start receiving from CAN
            m_ticker.start(MSG_CHECK_TIME);
            // Stop checking weight
            w_ticker.stop();
            state_before = ON;
        }
        if(state_before == IDLE && state == OFF) {
            // Kill lcd ticker
            lcd_ticker.stop();
            // Kill potenciometer ticker
            p_ticker.stop();
            // Stop checking temperature 
            temp_ticker.stop();
            state_before = OFF;
        }
        wait_ms(STATE_CHECK_TIME);
    }
}