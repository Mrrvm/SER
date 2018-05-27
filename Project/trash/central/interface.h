#include "mbed.h"
#include "codes.h"
#include "rtos.h"
#include "LM75B.h"
#include "HX711.h"
#include "C12832.h"
#include "pwm_tone.h"

#define FRONT
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


void temperatureHandler(int temp);
void distanceHandler(const uint16_t id, int dist);
void weightHandler(int weight);
void codeNotFoundHandler(CANMessage msg);
void canController(CANMessage msg);
void receive();
int send(uint16_t id, int message);
int temperatureSensor();
int weightSensor();

// Sensors
extern HX711 scale;
extern LM75B sensor;
extern CAN can; 
// Actuators
extern C12832 lcd;
extern PwmOut Buzzer;



