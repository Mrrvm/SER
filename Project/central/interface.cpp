#include "interface.h"
 
// Sensors
HX711 scale(W_SCK, W_DT);
LM75B sensor(TEMP_SDA, TEMP_SCL);
CAN can(CAN1RD, CAN1TD); 
AnalogIn pot(POT);
// Actuators
C12832 lcd(LCD_MOSI, LCD_SCK, LCD_RESET, LCD_A0, LCD_NCS);
PwmOut Buzzer(BUZZER);
// Others
Mutex m_buzzer;
Queue<LCD_msg, 8> LCD_queue;

/********************** START HANDLERS ************************/
/**
    temperature handler

    @param temp temperature received
*/
void temperatureHandler(int temp) {
    LCD_msg *msg;
    sprintf(msg->_string, "[Temperature] %d", temp);
    LCD_queue.put(msg);
}

/**
    potenciometer handler

    @param _pot potenciometers received
*/
void potHandler(int _pot) {
    LCD_msg *msg;
    sprintf(msg->_string, "[Potenciom] %d", _pot);
    LCD_queue.put(msg);
}

/**
    weight handler

    @param weight weight received
*/
void weightHandler(int weight) {
    LCD_msg *msg;
    sprintf(msg->_string, "[Weight] %d", weight);
    LCD_queue.put(msg);
}

/**
    front distance handler

    @param dist distance received
*/
void frontDistanceHandler(int dist) {
    
    int time, counter = 5;
    if(dist > DIST_L1) {
        time = 1000;
    }
    else if(dist > DIST_L2) {
        time = 500;
    }
    else {
        time = 200;
    }
    while(counter) {
        m_buzzer.lock();
        Tune(Buzzer, (float)1000000/Fa3, 4);
        m_buzzer.unlock();
        counter --;
        wait(time);
    }       
}
/**
    back distance handler

    @param dist distance received
*/
void backDistanceHandler(int dist) {
    
    int time, counter = 5;
    if(dist > DIST_L2) {
        time = 1000;
    }
    else if(dist > DIST_L3) {
        time = 500;
    }
    else {
        time = 200;
    }
    while(counter) {
        m_buzzer.lock();
        Tune(Buzzer, (float)1000000/Mi3, 4);
        m_buzzer.unlock();
        counter --;
        wait(time);
    } 
}

/**
    code error handler

    @param msg message received
*/
void codeNotFoundHandler(CANMessage *msg) {
    printf("[unknown] CAN message received");
    printf("  ID      = 0x%.3x\r\n", msg->id);
}

/********************** END HANDLERS ************************/
/********************** START RECEIVERS ************************/
/**
    generic controller to handle messages received in the CAN BUS

    @param message message received
*/
void canController(CANMessage *msg) {
    switch(msg->id) {      
        // FRONT DISTANCE SENSOR (from front, to central)
        case CODE_FRONT_DISTANCE:
            frontDistanceHandler(*reinterpret_cast<int*>(msg->data));
            break;
            
        // BACK DISTANCE SENSOR (from back, to central)
        case CODE_BACK_DISTANCE:
            backDistanceHandler(*reinterpret_cast<int*>(msg->data));
            break;
                
        default:
            codeNotFoundHandler(msg);
    }
    terminate();
}
/**
    generic function to receive messages through the CAN BUS

    @param can CAN interface to communicate
*/
int receive() {
    CANMessage msg;
    Thread t_handler;
    if(can.read(msg)) {
        t_handler.start(callback(canController, &msg));
        return 1;
    }
    printf("Error receiving message");
    return 0;
}

/********************** END RECEIVERS ************************/
/********************** START SENDERS ************************/


/**
    generic function to send messages through the CAN BUS

    @param can CAN interface to communicate
    @param id code of the message
    @param message message to be sent
    @return 1 if message sent, 0 otherwise
*/
int send(uint16_t id, int message) {
    if(!can.write(CANMessage(id, reinterpret_cast<char*>(&message), sizeof(message)))) {
        LCD_msg *msg;
        printf("Error sending message %x %d", id, message);
        sprintf(msg->_string, "[CAN] Cannot send messages");
        LCD_queue.put(msg);
        return 0;
    }
    return 1;
}
/********************** END SENDERS **************************/
/********************** START ANALYZERS ************************/

/**
    get current temperature from sensor
*/
int temperatureSensor() {
    if (sensor.open()) {
       return (int)sensor.temp();
    }
    return 100;
}


/**
    get current weight from sensor
*/
int weightSensor() {
    return static_cast<int>(scale.getGram());
}

/**
    get current weight from sensor
*/
int readPot() {
    return (int)pot;
}

/********************** END SENDERS ************************/