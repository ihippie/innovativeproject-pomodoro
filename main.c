/*
 * main.c
 *
 *  Created on: 17 mar 2015
 *      Author: Dulc
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "second.h"
#include "rs232.h"
#include "config.h"
#include "adc.h"

#include <string.h>
#include <stdio.h>


int config_t_pomodoro=1;
int config_t_przerwa1=1;
int config_t_przerwa2=15;
int config_n_kr_przerw=4;
int status_poziom_akumulatora=0;
int n_kr_przerw=1;
int button_flag=0;
extern char txt_buf2[32]="0";

volatile char button_handle_flag=0;



volatile stan_typedef stan=OCZEKIWANIE;
volatile stan_typedef nast_stan=POMODORO;
 stan_konfiguracja_typedef stan_konfiguracja=PRACA;

volatile char tmp=0;
void buttonHandler();


ISR(TIMER0_OVF_vect) // TIMER OBSLUGUJACY MULTIPLEKSOWANIE WYSWIETLACZA 7 segmentowego
{
	1+1;
}

ISR(TIMER1_COMPA_vect)	//TIMER GENERUJACY PRZERWANIA CO 1 SEKUNDE
{

		adcTrigger();

		second();
		if(minuty==0){
			switch(stan){
				case OCZEKIWANIE:
					sleepOn();
					//nast_stan=POMODORO;
					break;
				case POMODORO:
					pulsePin(16);

					if(n_kr_przerw>=config_n_kr_przerw){
						n_kr_przerw=0;
						nast_stan=PRZERWA2;
					}else{
						nast_stan=PRZERWA1;
					}
					break;
				case PRZERWA1:
					pulsePin(16);
//					minuty=2;
					n_kr_przerw++;
					nast_stan=POMODORO;
					break;
				case PRZERWA2:
					pulsePin(16);
//					minuty=2;
					nast_stan=POMODORO;
					break;

			}

			minuty=2;	//czas do uspienia urzadzenia
			#if DEBUGMODE ==1
				if(stan!=OCZEKIWANIE)
					RSTransmit("Zakonczono odcinek\n");
			#endif
			stan=OCZEKIWANIE;
		}

}

ISR(TIMER2_COMP_vect){	//TIMER GENERUJACY PRZERWANIA CO ~1/4 SEKUNDY, DO PIKANIA, MIGANIA WYSWIETLACZEM
	pulseObsluga();
}

ISR(INT0_vect)	//PRZERWANIE OD PRZYCISKU
{
	#if DEBUGMODE ==1
//		RSTransmit("\nWyzwolone INT0\n");

	#endif

	if(button_flag==0){// zabezpieczenie przed drganiami styków

		button_flag=3;//ustawienie flagi zabezpieczajacej przed drganiami stykow
		button_handle_flag=1;//ustawienie flagi obslugi przycisku w petli glownej



	}
}

ISR(USART_RXC_vect)
{

	tmp=UDR;


}

int main(void){


	secondInit();
	eintInit();
	pulseInit();
	RSInit();
	sleepInit();
	adcInit();
	sei();					//ODBLOKOWANIE PRZERWAN

//	loadConfig();


//	strcpy(txt_buf,"\n\n\n\nWitam Panstwa");
//	RSTransmit(txt_buf);
	RSTransmit("\n\n\n\nWitam Panstwa2");

//	saveConfig();
	//loadConfig();
	while(1) {
		if(tmp!=0){
			consoleCMD(tmp);
			tmp=0;
		}

		if(button_handle_flag==1){
			button_handle_flag=0;
			buttonHandler();
		}
	}
	return 0;
}



//KOMUNIKACJA PRZEZ KONSOLE RS232
void buttonHandler(){
//			RSTransmit("\nbutton action");
	clrLicznikN();//zerowanie licznika czasu od puszczenia przycisku
	if(stan_konfiguracja==PRACA){
			switch(nast_stan){
			case OCZEKIWANIE:
				stan=nast_stan;
				minuty=2;
				nast_stan=POMODORO;
				break;
			case POMODORO:
				stan=nast_stan;
				minuty=config_t_pomodoro;
				nast_stan=PRZERWA1;
				break;
			case PRZERWA1:
				stan=nast_stan;
				minuty=config_t_przerwa1;
				nast_stan=PRZERWA2;
				break;
			case PRZERWA2:
				stan=nast_stan;
				minuty=config_t_przerwa2;
				nast_stan=OCZEKIWANIE;
				break;
			}
			sekundy=0;

			//
			#if DEBUGMODE==1
				stanDecode(stan,txt_buf2);
				RSTransmit(txt_buf2);

				RSTransmit1i(minuty);
				RSTransmit("\n");
			#endif
	}else{

		#if DEBUGMODE==1
//							RSTransmit("\nKONFIGURACJA\n");
//							RSTransmit1i(stan_konfiguracja);
		#endif

		switch(stan_konfiguracja){

			case KONFIGURACJA_POM:
				config_t_pomodoro+=5;
				if(config_t_pomodoro>60)
					config_t_pomodoro-=60;
				RSTransmit1i(config_t_pomodoro);
			break;

			case KONFIGURACJA_PRZ1:
				config_t_przerwa1+=5;
				if(config_t_przerwa1>60)
					config_t_przerwa1-=60;
				RSTransmit1i(config_t_przerwa1);
			break;
			case KONFIGURACJA_PRZ2:
				config_t_przerwa2+=5;
				if(config_t_przerwa2>60)
					config_t_przerwa2-=60;
				RSTransmit1i(config_t_przerwa2);
			break;
			case KONFIGURACJA_N_KR_PRZ:
				config_n_kr_przerw+=1;
				if(config_n_kr_przerw>8)
					config_n_kr_przerw-=8;
				RSTransmit1i(config_n_kr_przerw);
			break;
			default:
			break;

		}
	}
}
