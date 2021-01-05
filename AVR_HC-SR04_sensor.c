/*
 * AVR_HC-SR04_sensor.c
 *
 * Created: 31.12.2020 13:38:11
 * Author : oxford
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "AVR_HC-SR04_sensor.h"
#include "mkuart.h"

/********************************      VARIABLES    *******************************************************/

volatile uint16_t g_PrevCount = 0;				//previous value of Timer1
volatile uint16_t g_PulseWidth = 0;				//width of echo pulse from sensor, in uS (micro seconds)
volatile uint8_t g_measurement_flag = 1;		//flag used in interruption, should be 1 at the beginning

/********************************     FUNCTIONS    ********************************************************/

void TIMER1_init()									//initializing of 16-bit Timer1
{
	TCCR1B = (1<<CS11)|(1<<ICES1);					//prescaler 8 and trigger on rising edge at the beginning
	TIMSK1 = (1<<ICIE1);							//Input Capture Mode on (interrupt from input)
}

/*****************************************************************************************************/

void HC_SR04_init()									//initializing of sensor work
{	
	TIMER1_init();									
	USART_Init(__UBRR);								//initializing USART transmision
	TRIGGER_DIR |= TRIGGER_BIT;						//setting trigger pin as output
		
}

/*****************************************************************************************************/

void measurement_start()							//starting of measurement
{
		TRIGGER_ON;
		_delay_us(10);								//high state by 10 us = triggering of measurement
		TRIGGER_OFF;
}

/*****************************************************************************************************/

uint16_t measurement_value()							//measured distance in cm
{
	if (g_PulseWidth > 30000)							//when PulseWidth is higher than sensor's range
	{
		return 10000;			
	}
	else if (g_measurement_flag < 1)					//checking if 2 edges passed
	{
		return 10000;		
	}
		
	return g_PulseWidth/58;
}

/*****************************************************************************************************/

void measurement_value_UART()		 					//sending measured distance in cm through UART
{
	uint16_t dist = measurement_value();
	
		if (dist > 9000)								//when PulseWidth is higher than sensor range
	{
		uart_puts("Out of range!");
		return;
	}
	
	uart_puts("Distance = ");
	uart_putint(dist,10);
	uart_puts("cm");
	uart_putc(0x0a);
}


/*****************************      INTERRUPTION          ********************************************/


ISR(TIMER1_CAPT_vect )									//remember to enable global interrupts [sei();]
{
	TCCR1B ^= (1<<ICES1);								//changing of edge (falling/rising) reaction
	uint16_t actual = fabs(ICR1);

	g_PulseWidth = actual - g_PrevCount;
	g_PrevCount = actual;

	if (g_measurement_flag > 1)							//automatic sending measured distance through UART, measurement value available only when 2 edges passed
	{
		measurement_value_UART();
		g_measurement_flag -= 2;						//deleting flag's status
	}

	g_measurement_flag ++;
}


