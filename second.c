
#include <avr/io.h>
#include "config.h"


#define SEC_IN_MIN 60	//MA BYC 60 !!!!!!!!!!!!!!!!!!
#define PRZYTRZYMANIE 5

int sekundy=0;
int minuty=0;
int tmp;
char licznikp=0;
char licznikn=0;


//inicjalizacja przerwania co 1 sekunde TIM2_COMP
void secondInit(){
	sekundy=0;
	minuty=0;

	TCCR1A =0;//1<<3;
	TCCR1B |=3<<0|1<<3;	//ustawienie prescalera /64


	tmp=(F_CPU/64);
	OCR1AH=tmp>>8;//0x3d
	OCR1AL=tmp;//0x09

	TIMSK|=1<<4;//odblokowanie przewania
}

//
void second(){

	sekundy++;
	if(sekundy>=SEC_IN_MIN){
		sekundy=0;
		if(minuty>0){
			minuty--;
		}
	}

	//licznik przytrzymania klawisza
	if((PIND>>2)&1){
		licznikp=0;
		licznikn++;
		if(licznikn>=PRZYTRZYMANIE){
			licznikn=0;
			przytrzymanieN();
		}
	}else{

		licznikp++;
		licznikn=0;
		if(licznikp>=PRZYTRZYMANIE){
			licznikp=0;
			przytrzymanieP();
		}
	}

}
void clrLicznikN(){
	licznikn=0;
}
void przytrzymanieP(){
	RSTransmit("\nKONFIG");
	stan_konfiguracja=KONFIGURACJA_POM;
}
void przytrzymanieN(){
	//	RSTransmit("\npuszczony przycisk przez 5s");
	if(stan_konfiguracja!=PRACA){
		switch(stan_konfiguracja){

			case KONFIGURACJA_POM:
				stan_konfiguracja=KONFIGURACJA_PRZ1;
				RSTransmit("\nconf prz1");
			break;

			case KONFIGURACJA_PRZ1:
				stan_konfiguracja=KONFIGURACJA_PRZ2;
				RSTransmit("\nconf prz2");
			break;
			case KONFIGURACJA_PRZ2:
				stan_konfiguracja=KONFIGURACJA_N_KR_PRZ;
				RSTransmit("\nconf nkrprz");
			break;
			default:
				stan_konfiguracja=PRACA;
				RSTransmit("\nconf end");
			break;

		}

	}
}
