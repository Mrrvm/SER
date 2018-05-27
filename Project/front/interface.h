#include "mbed.h"
#include "codes.h"
#include "rtos.h"
#include "HCSR04.h"
#include "PowerControl/PowerControl.h"
#include "PowerControl/EthernetPowerControl.h"

#define RAIN_OFF 20
#define RAIN_L1 50
#define RAIN_L2 70
#define RAIN_L3 71
#define DEFAULT_DIST 30
#define DEFAULT_LIGHT 76
#define SLEEP 1
#define ON 2
#define LIGHTS_ON 1
#define LIGHTS_OFF 0
#define MSG_CHECK_TIME 500
#define DIST_CHECK_TIME 500
#define STATE_CHECK_TIME 1000
#define SLEEP_CHECK_TIME 2000
#define LDR_CHECK_TIME 2000

void LDRHandler(int lum);
void rainHandler(int amount);
void codeNotFoundHandler(CANMessage msg);
void canController(CANMessage msg);
int receive();
int send(uint16_t id, int message);
int LDRSensor();
int distanceSensor();

// Sensors
extern AnalogIn photocell;
extern HCSR04 mu;                                          
extern CAN can; 
// Actuators
extern DigitalOut led1;
// Others
extern int state;


