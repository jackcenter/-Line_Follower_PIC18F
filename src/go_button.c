/*
 * File:   go_button.c
 * Author: Jack
 *
 * Created on November 18, 2020, 8:15 PM
 */

#include <xc.h>
#include <pic18f87k22.h>
#include <ir_sensors.h>
#include <motors.h>
#include <go_button.h>

#define GO_T TRISB0
#define GO_P PORTB0

void init_go_button(){
    TRISBbits.GO_T = 1;
    
    INTCONbits.INT0IF = 0;  // Clear flag
    INTCONbits.INT0IE = 1;  // Enable (always high pri)
}

char execute_delivery(){
    // could add a light thing here
	start_ADC();
	motors_drive(0, 0)
	motors_engage();

	return 1;	// robot is running
}

char enter_sleep_mode(){
	motors_disengage();
	stop_ADC()
	// could add a light thing here
	display_value = 0;
	Sleep();

	return 0;	// robot was asleep
}

char pause_delivery(){
	motors_brake();
	motors_disengage();

	return 0;	// robot is stopped, will continue on next push
}
