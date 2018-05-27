#include "interface.h"
 
// Sensors
AnalogIn rain(RAIN);
HCSR04 mu(MU_BACK_TRIGGER, MU_BACK_ECHO); 
CAN can(CAN1RD, CAN1TD); 
// Actuators
DigitalOut led1(LED_BACK_1);

/********************** START HANDLERS ************************/

/**
    LDR handler

    @param lum luminosity received
*/
void LDRHandler(int lum) {
    if(lum < DEFAULT_LIGHT) {
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
    
    if(amount < RAIN_L1) {
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
    printf("[error] CAN message received");
    printf("  ID      = 0x%.3x\r\n", msg.id);
    /*
    printf("  Type    = %d\r\n", msg.type);
    printf("  Format  = %d\r\n", msg.format);
    printf("  Length  = %d\r\n", msg.len);
    printf("  Data    =");            
    for(int i = 0; i < msg.len; i++)
        printf(" %.2x", msg.data[i]);
    printf("End Message\r\n");
    */
}

/********************** END HANDLERS ************************/
/********************** START RECEIVERS ************************/

/**
    generic controller to handle messages received in the CAN BUS

    @param message message received
*/
void canController(CANMessage msg) {

    switch(msg.id) {      

        // LDR SENSOR (from front, to back)
        case CODE_LDR:
            LDRHandler(*reinterpret_cast<int*>(msg.data));
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
        printf("Recieved\n");
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
    if(!can.write(CANMessage(id, reinterpret_cast<char*>(&message), sizeof(message)))) {
        printf("Error sending message %x %d\n", id, message);
        return 0;
        // TODO if something is wrong tell the other nodes that something is wrong?
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
    get and send the current rain amount from sensor
*/
int rainSensor() {
    return (int)(100-rain.read()*100.0f);
}




/********************** END SENDERS ************************/
/********************** START AUXILIARIES ************************/


/********************** END AUXILIARIES ************************/