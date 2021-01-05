/*
 * AVR_HC-SR04_sensor.c
 *
 * Created: 31.12.2020 13:38:11
 * Author : oxford
 * This simple library is destinated to 8-bit AVR MCU's. 
 * I used Atmega 88PA to run it.
 * It use another UART library mkuart, but it can be easy changes of course.
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "mkuart.h"

// interrupt vectors: "avr/iom88pa.h"


#ifndef AVR_HCSR04_sensor_H_
#define AVR_HCSR04_sensor_H_

/******************************** SETTING CONNECTION PINS  *************************************************/

#define TRIGGER_PORT PORTD								//direction register of trigger
#define TRIGGER_DIR DDRD								//direction register of trigger
#define TRIGGER_BIT (1<<PD6)							//address of trigger, start of measurement, 



/********************************      MACROS       *******************************************************/

#define TRIGGER_ON TRIGGER_PORT |= TRIGGER_BIT
#define TRIGGER_OFF TRIGGER_PORT &= ~TRIGGER_BIT



/********************************      VARIABLES    *******************************************************/

extern volatile uint16_t g_PrevCount;				//previous value of Timer1
extern volatile uint16_t g_PulseWidth;				//width of Echo pulse from sensor, in uS (micro seconds)
extern volatile uint8_t g_measurement_flag;			//flag used in interruption 




/********************************      DECLARATIONS OF FUNCTIONS    ****************************************/

void TIMER1_init();									//initializing of 16-bit Timer1
void HC_SR04_init();								//initializing of sensor work (includes UART & Timer1 init)

void measurement_start();							//starting of measurement
uint16_t measurement_value();						//measured distance in cm 
void measurement_value_UART();		 				//sending measured distance in cm through UART




#endif /* AVR_HCSR04_sensor_H_ */

