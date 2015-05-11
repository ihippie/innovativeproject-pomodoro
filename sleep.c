/*
 * sleep.c
 *
 *  Created on: 17 kwi 2015
 *      Author: Dulc
 */
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "rs232.h"

void sleepInit(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	MCUCR|=1<SM1; //powerdown mode
}
void sleepOn(){
	char wakeup_flag=0;
	do{
		RSTransmit("Sleep.......");


		cli(); //zablokowanie przerwan
		MCUCR&=~(1<<ISC01|1<<ISC00);//ustawien ie przerwania od INT0 na stan niski


		sleep_enable();
		sei();		//odblokowanie przerwan


		sleep_cpu();//uspienie procesora


		cli();		//procesor budzi sie tu, wylaczenie przerwan


		sleep_disable();	//wylaczenie mozliwosci uspienia

		RSTransmit("\nConfirm\n");
		_delay_ms(2000);


		wakeup_flag=(PIND>>2)&1;	//sprawdzenie czy przycisk jest nadal wcisniety po 2000 ms
		if(!wakeup_flag){
			RSTransmit("OK\n");		//jesli tak - procesor zostanie wybudzony

		}else{
			RSTransmit("Not confirmed\n"); //jesli nie - procesor zostanie ponownie uspiony

		}
		RSTransmit1i(wakeup_flag);
	}while(wakeup_flag);

	RSTransmit("OK\n");
	RSTransmit("Waking up...\n");
	MCUCR|=(1<<ISC01|0<<ISC00);				// ustawienie przerwania INT0 ponownie na zbocze
	sei();									//odblokowanie przerwan





}
