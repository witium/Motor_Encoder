/*
 * PID.c
 *
 *  Created on: ١٣‏/٠٢‏/٢٠١٧
 *      Author: El-safi
 */
#include "lcdEKE.h"
#include "PWM1_EKE.h"
#include "Timer0.h"
#include "External_Enterrupt.h"
#include <util/delay.h>
volatile  U16_t revolution=0,revolution_overFlow=0;  // make compiler not optimize revolution and save it
volatile  U16_t Minutes=0,secs_overFlow=0,seconds=0;
volatile  U16_t actualRPM=0;
int main(void){
	/*system data types */
	DDRD|=(1<<PD4);             // enable pin B3 for PWM
	DDRC|=(1<<PC0)|(1<<PC1);   // motor direction control
	PORTC|=(1<<PC0);
	PORTC&=~(1<<PC1);
	/*system initialization for modules */
	EKE_LCD_INIT();
    externalEnterrupt_init();
    timer0_interrupt_init();
    sei();                       // enable global interrupt
    PWM1_EKE_init();
    PWM1_EKE_start();

	while(1){


		      goToRowColumn(1,1);EKE_LCD_string((U8_t*)"MOTOR SPEED ");
              goToRowColumn(2,1);EKE_LCD_string((U8_t*)"RPM:");
              goToRowColumn(2,5);
              EKE_LCD_intNumASCII(actualRPM);
              PWM1_EKE_duty(255);


	}

	return 0;
}

      /*          ****  ISR for system  ****        */

// external interrupt to count number of pulses (every 24 pulse = 1 revolution )
ISR(INT1_vect){

    revolution_overFlow++;        // increament the number of revolutions tha hit int1 at rising mode
    if(revolution_overFlow==24){
    	revolution++;
    	revolution_overFlow=0;
    }
}
// timer overflow interrupt to calculate 1 seconde
ISR(TIMER0_OVF_vect){
	secs_overFlow++;
	 if(secs_overFlow>=31){  // 1 seconde passed .. 1 tick=64 usec (1 sec =61 overflow) so (1 minute =61*60 overflow)
	   secs_overFlow=0;
	   TCNT0=0;
       revolution=revolution*60;
       actualRPM=revolution;
       revolution=0;

	  }

}
