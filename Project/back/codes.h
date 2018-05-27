#ifndef CODES_H
#define CODES_H

/********** Central ************/
// buzzer
#define BUZZER p26
// weight
#define W_SCK p13
#define W_DT p12
// temperature
#define TEMP_SCL p27
#define TEMP_SDA p28
// can
#define CAN1RD p9
#define CAN1TD p10
// lcd 
#define LCD_MOSI p5
#define LCD_SCK p7
#define LCD_RESET p6
#define LCD_A0 p8
#define LCD_NCS p11
// potenciometer
#define POT p19
/********** Front and Back ************/
// back distance
#define MU_BACK_TRIGGER p7 
#define MU_BACK_ECHO p8
// front distance
#define MU_FRONT_TRIGGER p7
#define MU_FRONT_ECHO p8
// back rain
#define RAIN p20
// back leds
#define LED_BACK_1 p5 
// front leds
#define LED_FRONT_1 p5 
// ldr
#define LDR_PIN p20

const uint16_t  CODE_INTERRUPT = 0x200;
const uint16_t  CODE_FRONT_DISTANCE = 0x201;    
const uint16_t  CODE_BACK_DISTANCE = 0x202;     
const uint16_t  CODE_LDR = 0x203;
const uint16_t  CODE_RAIN = 0x204;    

#endif