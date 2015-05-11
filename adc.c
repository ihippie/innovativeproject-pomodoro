/*
 * adc.c
 *
 *  Created on: 8 kwi 2015
 *      Author: Dulc
 */
#include <avr/io.h>
#include "config.h"

void adcInit(void){
	ADMUX|=7|1<<ADLAR;
	ADCSRA|=1<<ADEN|7;
	SFIOR|=1<<ADTS2|1<<ADTS0;
}
void adcTrigger(void){
	//odczyt poziomu napiecia na akumulatorze
	if(!((ADCSRA>>ADSC)&1)){
		status_poziom_akumulatora=ADCH*100/255;
		ADCSRA|=1<<ADSC;
	}
}
