#include "interface.h"
 

// Sensors
AnalogIn photocell(LDR_PIN);
HCSR04 mu(MU_FRONT_TRIGGER, MU_FRONT_ECHO);                                          
CAN can(CAN1RD, CAN1TD); 
// Actuators
DigitalOut led1(LED_FRONT_1);


/**
    LDR handler

    @param lum luminosity received
*/
void LDRHandler(int lum) {
    if(lum==1){
        led1.write(1);
        printf("Lights are on");
    }else{
        led1.write(0);
        printf("Lights are off");
    } 
}

/**
    rain handler

    @param amount amount of rain received
*/
void rainHandler(int amount) {
    
    if(amount > RAIN_L1) {
        printf("Windshield cleaners OFF");
    } 
    else if(amount < RAIN_L2) {
        printf("Windshield cleaners LEVEL 1");
    }
    else if(amount < RAIN_L3){
        printf("Windshield cleaners LEVEL 2");
    }
    else {
        printf("Windshield cleaners LEVEL 3");
    }
}


/**
    code error handler

    @param msg message received
*/
void codeNotFoundHandler(CANMessage msg) {
    printf("[error] CAN message received\r\n");
    printf("  ID      = 0x%.3x\r\n", msg.id);
    printf("  Type    = %d\r\n", msg.type);
    printf("  Format  = %d\r\n", msg.format);
    printf("  Length  = %d\r\n", msg.len);
    printf("  Data    =");            
    for(int i = 0; i < msg.len; i++)
        printf(" %.2x", msg.data[i]);
    printf("End Message\r\n");
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
                
        default:
            codeNotFoundHandler(msg);
    }

}


/**
    generic function to receive messages through the CAN BUS

    @param can CAN interface to communicate
*/
void receive() {
    CANMessage msg;
    if(can.read(msg)) {
        canController(msg);
    }
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
    if(!can.write(CANMessage(id, reinterpret_cast<char*>(&message), 1))) {
        printf("Error sending message %x %d", id, message);
        return 0;
        // TODO if something is wrong tell the other nodes that something is wrong?
    }
    return 1;
}
/********************** END SENDERS **************************/
/********************** START ANALYZERS ************************/

/**
    get current luminosity from sensor
*/
int LDRSensor() {
    return photocell;
}


/**
    get and send the current distance from sensor
*/
int distanceSensor() {
    return (int)mu.getCm();
}


/********************** END SENDERS ************************/
/********************** START AUXILIARIES ************************/


/********************** END AUXILIARIES ************************/