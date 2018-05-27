#include "mbed.h"
#include "codes.h"
#include "rtos.h"
#include "LM75B.h"
#include "HX711.h"
#include "C12832.h"
#include "pwm_tone.h"
#include "PowerControl/PowerControl.h"
#include "PowerControl/EthernetPowerControl.h"

#define DIST_L1 20
#define DIST_L2 10
#define DIST_L3 5
#define DEFAULT_TEMP 22
#define DEFAULT_WEIGHT 1 //1Kg
#define DEFAULT_POT 20
#define OFF 1
#define IDLE 2
#define ON 3
#define WEIGHT_CHECK_TIME 5000
#define TEMP_CHECK_TIME 5000
#define MSG_CHECK_TIME 500
#define POT_CHECK_TIME 500
#define LCD_CHECK_TIME 4000
#define STATE_CHECK_TIME 1000

void temperatureHandler(int temp);
void distanceHandler(const uint16_t id, int dist);
void weightHandler(int weight);
void potHandler(int _pot);
void codeNotFoundHandler(CANMessage msg);
int receive();
int send(uint16_t id, int message);
int temperatureSensor();
int weightSensor();
int readPot();

// Sensors
extern HX711 scale;
extern LM75B sensor;
extern CAN can; 
extern AnalogIn pot;
// Actuators
extern C12832 lcd;
extern PwmOut Buzzer;
// Others
typedef struct _LCD_msg {
    char _string[30];
} LCD_msg;
extern Mutex m_buzzer;
extern Queue<LCD_msg, 8> LCD_queue;

