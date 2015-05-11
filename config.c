/*
 * config.c
 *
 *  Created on: 13 kwi 2015
 *      Author: Dulc
 */
#include "config.h"
#include "rs232.h"
#include <avr/eeprom.h>

void stanDecode(stan_typedef stan,char *txt_buf){
	if(stan==POMODORO){
		strcpy(txt_buf,"POMODORO\n");
	}else if(stan==PRZERWA1){
		strcpy(txt_buf,"PRZERWA1\n");
	}else if(stan==PRZERWA2){
		strcpy(txt_buf,"PRZERWA2\n");
	}else if(stan==OCZEKIWANIE){
		strcpy(txt_buf,"OCZEKIWANIE\n");
	}
}

void saveConfig(){
	uint16_t *__p=0;
	eeprom_write_word(__p,config_t_pomodoro);
	eeprom_write_word(__p+1,config_t_przerwa1);
	eeprom_write_word(__p+2,config_t_przerwa2);
	eeprom_write_word(__p+3,config_n_kr_przerw);
}
void loadConfig(){
	uint16_t *__p=0;
	config_t_pomodoro = eeprom_read_word(__p);
	config_t_przerwa1 = eeprom_read_word(__p+1);
	config_t_przerwa2 = eeprom_read_word(__p+2);
	config_n_kr_przerw = eeprom_read_word(__p+3);
}
