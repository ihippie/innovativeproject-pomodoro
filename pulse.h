/*
 * pulse.h
 *
 *  Created on: 13 mar 2015
 *      Author: Dulc
 */

volatile int pulse_display_counter0;
int pulse_display_slow;

void pulseInit();
void pulseDisplayBlink(int n);
void pulseDisplaySlow();
void pulseObsluga();
void pulsePin(int n);
