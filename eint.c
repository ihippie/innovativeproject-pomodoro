/*
 * eint.c
 *
 *  Created on: 20 mar 2015
 *      Author: Dulc
 */

#include <avr/interrupt.h>
void eintInit(){
	PORTD|=1<<2;// ustawienie wewnetrzenego rezystora podciagajacego
	 MCUCR|=(1<<ISC01|0<<ISC00);		//przerwanie na zbocze opadajace
	 GICR|=1<<6;//odblokowanie przerwania
}
