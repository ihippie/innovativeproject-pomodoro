/*
 * pulse.c
 *
 *  Created on: 13 mar 2015
 *      Author: Dulc
 */
#include <avr/io.h>
#include "config.h"
#include "rs232.h"

volatile int pulse_display_counter0=9;
int pulse_display_slow=0;
int pulse_pin_counter=4;
char pulse_counter=0;

void pulseInit(){
	TCCR2|=7//prescaler /1024
			|1<<3;//wlaczenie trybu ctc
	TIMSK|=1<<7;
	OCR2=(1000000/8);
	DDRD|=1;
}

void pulseDisplayBlink(int n){
	pulse_display_counter0=n;
}

void pulseDisplaySlow(){
	pulse_display_slow^=1;
}
void pulsePin(int n){
	pulse_pin_counter=n;
}

void pulseObsluga(){
	pulse_counter++;

	if(pulse_counter%1==0){
		if(pulse_display_counter0>0){
			PORTB^=1<<2;
//			segment_display_mask^=3;
			pulse_display_counter0--;
			if(pulse_display_counter0==0){
//				segment_display_mask=3;
			}
		}
	}
	if(pulse_counter%4==0){
		if(pulse_display_counter0==0){
			if(pulse_display_slow==1){
//				segment_display_mask^=3;
			}
		}
	}
	if(pulse_counter%1==0){
		pulse_pin_counter--;
		if(pulse_pin_counter>0){
			PORTD^=1;
		}else{
			PORTD&=~1;
		}


	}

	if(pulse_counter%2==0){
		button_flag&=~(1<<1);
	}else if(pulse_counter%2==1){
		button_flag&=~(1<<0);
	}

}
