/*
 * rs232.h
 *
 *  Created on: 13 kwi 2015
 *      Author: Dulc
 */

#ifndef RS232_H_
#define RS232_H_




void RSInit();
void int2string(int integer,char* out_string);
void RSTransmit(char *txt_buf);
void RSTransmit1i(int number);
void consoleCMD(int cmd);

#endif /* RS232_H_ */
