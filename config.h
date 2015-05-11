/*
 * config.h
 *
 *  Created on: 13 kwi 2015
 *      Author: Dulc
 */

#ifndef CONFIG_H_
#define CONFIG_H_


#define DEBUGMODE 1
#define MIN_VAL 0
#define F_CLK 1000000
#define VERSION "1.3"

extern char txt_buf2[32];
extern	int config_t_pomodoro;
extern	int config_t_przerwa1;
extern	int config_t_przerwa2;
extern	int config_n_kr_przerw;
extern	int status_poziom_akumulatora;



extern	int button_flag;
extern	int n_kr_przerw;

typedef enum {POMODORO,PRZERWA1,PRZERWA2,OCZEKIWANIE} stan_typedef;
typedef enum {PRACA,KONFIGURACJA_POM,KONFIGURACJA_PRZ1,KONFIGURACJA_PRZ2,KONFIGURACJA_N_KR_PRZ} stan_konfiguracja_typedef;


volatile stan_typedef stan;
volatile stan_typedef nast_stan;
 stan_konfiguracja_typedef stan_konfiguracja;

//extern struct config;
//extern struct config conf;

void stanDecode(stan_typedef stan,char *txt_buf);

void saveConfig();
void loadConfig();
#endif /* CONFIG_H_ */
