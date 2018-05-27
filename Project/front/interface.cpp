#include "interface.h"
 
// Sensors
AnalogIn photocell(LDR_PIN);
HCSR04 mu(MU_FRONT_TRIGGER, MU_FRONT_ECHO); 
CAN can(CAN1RD, CAN1TD); 
// Actuators
DigitalOut led1(LED_FRONT_1);
// Others
int state = SLEEP;


/********************** START HANDLERS ************************/

/**
    Interrupt handler

    @param slave_state sleep/wakeup state received
*/
void interruptHandler(int slave_state) {
    if(slave_state) {
        state = ON;
    }
    else {
        state = SLEEP;
    }
}

/**
    LDR handler

    @param lum luminosity received
*/
void LDRHandler(int lum) {
    if(lum) {
        led1.write(LIGHTS_ON);
    }else{
        led1.write(LIGHTS_OFF);
    } 
    printf("[Lights] %d", lum); 
}


/**
    Rain handler

    @param amount amount of rain received
*/
void rainHandler(int tag) {
    if(tag == RAIN_OFF) {
        printf("[Windshield cleaners] OFF");
    } 
    else if(tag == RAIN_L1) {
        printf("[Windshield cleaners] LEVEL 1");
    }
    else if(tag == RAIN_L2){
        printf("[Windshield cleaners] LEVEL 2");
    }
    else {
        printf("[Windshield cleaners] LEVEL 3");
    } 
}


/**
    code error handler

    @param msg message received
*/
void codeNotFoundHandler(CANMessage msg) {
    printf("[unknown] CAN message received");
    printf("  ID      = 0x%.3x\r\n", msg.id);
}

/********************** END HANDLERS ************************/
/********************** START RECEIVERS ************************/

/**
    generic controller to handle messages received in the CAN BUS

    @param message message received
*/
void canController(CANMessage msg) {

    switch(msg.id) {      
        // RAIN SENSOR (from back, to front)
        case CODE_RAIN:
            rainHandler(*reinterpret_cast<int*>(msg.data));
            break;  
        // INTERRUPT SLEEP (from cental, to back)
        case CODE_INTERRUPT:
            interruptHandler(*reinterpret_cast<int*>(msg.data));
            break;                
        default:
            codeNotFoundHandler(msg);
    }

}


/**
    generic function to receive messages through the CAN BUS

    @param can CAN interface to communicate
*/
int receive() {
    CANMessage msg;
    if(can.read(msg)) {
        canController(msg);
        return 1;
    }
    printf("[CAN] Cannot send messages");
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
        printf("Error sending message %x %d\n", id, message);
        return 0;
    }
    return 1;
}
/********************** END SENDERS **************************/
/********************** START ANALYZERS ************************/

/**
    get and send the current distance from sensor
*/
int distanceSensor() {
    return (int)mu.getCm();
}


/**
    get current luminosity from sensor
*/
int LDRSensor() {
    return photocell;
}

/********************** END SENDERS ************************/