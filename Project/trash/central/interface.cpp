#include "interface.h"
 
// Sensors
HX711 scale(W_SCK, W_DT);
LM75B sensor(TEMP_SDA, TEMP_SCL);
CAN can(CAN1RD, CAN1TD); 
// Actuators
//C12832 lcd(p5, p7, p6, p8, p11);
PwmOut Buzzer(BUZZER);


/********************** START HANDLERS ************************/

/**
    temperature handler

    @param temp temperature received
*/
void temperatureHandler(int temp) {
    /*
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("Temperature diff: %d", temp);*/
    wait_ms(4000);
}

/**
    front distance handler

    @param dist distance received
*/
void frontDistanceHandler(int dist) {
    /*
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("Front Distance WARNING!");*/
    if(dist > DIST_L2) {
        Tune(Buzzer, (float)1000000/Mi3, 4);
    }
    else if(dist > DIST_L3) {
        Tune(Buzzer, (float)1000000/Mi4, 4);
    }
    else {
        Tune(Buzzer, (float)1000000/Mi5, 4);
    }
         
}

/**
    back distance handler

    @param dist distance received
*/
void backDistanceHandler(int dist) {
    /*
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("Back Distance WARNING!");*/
    if(dist > DIST_L2) {
        Tune(Buzzer, (float)1000000/Fa3, 4);
    }
    else if(dist > DIST_L3) {
        Tune(Buzzer, (float)1000000/Fa4, 4);
    }
    else {
        Tune(Buzzer, (float)1000000/Fa5, 4);
    }
}


/**
    front distance handler

    @param dist distance received
*/
void weightHandler(int weight) {
    /*
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("Closing car");*/
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

        // FRONT DISTANCE SENSOR (from front, to central)
        case CODE_FRONT_DISTANCE:
            frontDistanceHandler(*reinterpret_cast<int*>(msg.data));
            break;
            
        // BACK DISTANCE SENSOR (from back, to central)
        case CODE_BACK_DISTANCE:
            backDistanceHandler(*reinterpret_cast<int*>(msg.data));
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
    if(!can.write(CANMessage(id, reinterpret_cast<char*>(&message), sizeof(message)))) {
        printf("Error sending message %x %d", id, message);
        return 0;
        // TODO if something is wrong tell the other nodes that something is wrong?
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



/********************** END SENDERS ************************/
/********************** START AUXILIARIES ************************/


/********************** END AUXILIARIES ************************/