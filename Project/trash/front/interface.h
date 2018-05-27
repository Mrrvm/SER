#include "mbed.h"
#include "codes.h"
#include "rtos.h"
#include "HCSR04.h"
#include "LM75B.h"
#include "HX711.h"
#include "C12832.h"
#include "pwm_tone.h"

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
#define DEFAULT_RAIN 0

void LDRHandler(int lum);
void rainHandler(int amount);
void codeNotFoundHandler(CANMessage msg);
void canController(CANMessage msg);
void receive();
int send(uint16_t id, int message);
int LDRSensor();
int distanceSensor();

// Sensors
extern AnalogIn photocell;
extern HCSR04 mu;                                          
extern CAN can; 
// Actuators
extern DigitalOut led1;


