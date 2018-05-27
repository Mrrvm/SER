#include "mbed.h"
#include "codes.h"
#include "rtos.h"
#include "HCSR04.h"

#define RAIN_L1 30
#define RAIN_L2 60
#define RAIN_L3 100
#define DIST_L1 30
#define DIST_L2 15
#define DIST_L3 7
#define DEFAULT_TEMP 22
#define DEFAULT_WEIGHT 0
#define DEFAULT_LIGHT 100
#define DEFAULT_DIST 30
#define DEFAULT_RAIN 20

void LDRHandler(int lum);
void rainHandler(int amount);
void codeNotFoundHandler(CANMessage msg);
void canController(CANMessage msg);
void receive();
int send(uint16_t id, int message);
int rainSensor();
int distanceSensor();

// Sensors
extern AnalogIn rain;
extern HCSR04 mu; 
extern CAN can; 
// Actuators
extern DigitalOut led1;


