/*
 * rs232.c
 *
 *  Created on: 13 kwi 2015
 *      Author: Dulc
 */
#include <avr/io.h>
#include "config.h"
#include "second.h"



#define BAUD_RATE 2400
#define BAUD_REG F_CLK/(8*BAUD_RATE)-1


//char txt_buf_rs232[64];
void RSInit(){
	//UART INIT
	UCSRA|=1<<1; //U2X enable

	//UBRRL|=0x33;//baud rate 2400
	UBRRL|=BAUD_REG;

	UCSRC|=1<<URSEL|1<<USBS|
			1<<2|1<<1;// transmisja 8 bitowa

	UCSRB|=1<<3|1<<4|// TXenable RX enable
			1<<RXCIE;// odblokowanie przerwan

}

void int2string(int integer,char* out_string){
	// funkcja pobierajaca wartosc uint i wpisuyj¹ca pod podan¹ tablice znaków wartoci ascii
	// zakres 0-9999
	// 4 znaki

	char znaki[10] ="0123456789";
	out_string[0]=znaki[(integer/1000)%10];
	out_string[1]=znaki[(integer/100)%10];
	out_string[2]=znaki[(integer/10)%10];
	out_string[3]=znaki[integer%10];
	out_string[4]='\0';
}

void RSTransmit(char *txt_buf){
	int i=0;
	while(txt_buf[i]){
		if((UCSRA>>UDRE)&1==1){
			UDR=txt_buf[i];

			i++;
		}

	}
}

void RSTransmit1i(int number){
	int2string(number, txt_buf2);
	int i=0;
	while(txt_buf2[i]){
		if((UCSRA>>UDRE)&1==1){
			UDR=txt_buf2[i];

			i++;
		}

	}
}


void consoleCMD(int cmd){

	int Hpart=(cmd&0xF0)>>4;
	int Lpart=cmd&0x0F;

	switch(Hpart){
	case 0x2:
		//INFO

		RSTransmit("\n\nPOMODORO v.");
		RSTransmit(VERSION);
		RSTransmit("  \nSTAN:		");

		stanDecode(stan,txt_buf2);
		RSTransmit(txt_buf2);


		RSTransmit("NAST_STAN:		");

		stanDecode(nast_stan,txt_buf2);
		RSTransmit(txt_buf2);

		RSTransmit("N_KR_PRZERW:		");
		RSTransmit1i(n_kr_przerw);


		RSTransmit("\nMINUTY:		");
		int2string(minuty,txt_buf2);
		RSTransmit(txt_buf2);


		RSTransmit("\nSEKUNDY:		");
		RSTransmit1i(sekundy);


		RSTransmit("\nCONFIG_T_POMODORO:		");
		RSTransmit1i(config_t_pomodoro);

		RSTransmit("\nCONFIG_T_PRZERWA1:		");
		RSTransmit1i(config_t_przerwa1);

		RSTransmit("\nCONFIG_T_PRZERWA2:		");
		RSTransmit1i(config_t_przerwa2);

		RSTransmit("\nCONFIG_N_KR_PRZERW:		");
		RSTransmit1i(config_n_kr_przerw);

		RSTransmit("\nstatus_poziom_akumulatora:		");
		RSTransmit1i(status_poziom_akumulatora);

		break;

	case 0x3:
		int2string(Lpart,txt_buf2);
		RSTransmit(txt_buf2);
		break;
	case 0x4:
		//Ustawienie config_t_pomodoro
		config_t_pomodoro=Lpart*5+MIN_VAL;

		RSTransmit("Ustawiono:\nCONFIG_T_POMODORO:		");
		RSTransmit1i(config_t_pomodoro);
		break;
	case 0x5:
		//Ustawienie config_t_przerwa1
		config_t_przerwa1=Lpart+MIN_VAL;

		RSTransmit("Ustawiono:\nCONFIG_T_PRZERWA1:		");
		RSTransmit1i(config_t_przerwa1);
		break;
	case 0x6:
		//Ustawienie config_t_przerwa2
		config_t_przerwa2=Lpart*5+MIN_VAL;

		RSTransmit("Ustawiono:\nCONFIG_T_PRZERWA2:		");
		RSTransmit1i(config_t_przerwa2);
		break;

	case 0x7:
		//Ustawienie config_t_przerwa2
		config_n_kr_przerw=Lpart;

		RSTransmit("Ustawiono:\nCONFIG_N_KR_PRZERW:		");
		RSTransmit1i(config_n_kr_przerw);
		break;
	case 0xe:
			//Ustawienie config_t_przerwa2
			loadConfig();

			RSTransmit("Zaladowano ustawienia z EEPROM");

			break;
	case 0xd:
			//Ustawienie config_t_przerwa2
			saveConfig();

			RSTransmit("Zapisano ustawienia z EEPROM");

			break;



	case 0xf:
		//help
		RSTransmit("0x2:		INFO\n"
				"0x4t:		Set pomodoro duration time as (t*5+5)\n"
				"0x5t:		Set short break duration time as (t+5)\n"
				"0x6t:		Set short long duration time as (5*t+5)\n"
				"0x7n:		Set number of short breaks between long one as n\n");
		break;

	default:
		strcpy(txt_buf2,"Unknown command \nFor help insert 0xff");
		RSTransmit(txt_buf2);
		break;

	}
	RSTransmit("\n");
}


