/*
 * File:   go_button.c
 * Author: Jack
 *
 * Created on November 18, 2020, 8:15 PM
 */

#include <xc.h>
#include <pic18f87k22.h>
#include <go_button.h>

#define _XTAL_FREQ 16000000

#define GO_T TRISB0
#define GO_P PORTB0

extern char display_value;

void init_go_button(){
    TRISBbits.GO_T = 1;
    INTCON2bits.INTEDG0 = 1;
            
    enable_go_button();
}

void enable_go_button(){
    INTCONbits.INT0IF = 0;  // Clear flag
    INTCONbits.INT0IE = 1;  // enable (always high pri)
}

void disable_go_button(){
    INTCONbits.INT0IE = 0;  // Disable (always high pri)
    INTCONbits.INT0IF = 0;  // Clear flag
}

char execute_delivery(){
    // could add a light thing here
    static char x[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
                       0x00};
    
    for (int i = 9; i >= 0; --i){
            load_byte(x[i]);
            __delay_ms(100);
    }
    
	start_ADC();
	motors_drive(0, 0);
	motors_engage();

	return 1;	// robot is running
}

char enter_sleep_mode(){
	motors_disengage();
	stop_ADC();
	// could add a light thing here
	display_value = 0;
	Sleep();

	return 0;	// robot was asleep
}

char pause_delivery(){
    motors_brake();
//	motors_disengage();
    stop_ADC();
    
    static char x[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
                       0x00};
    
    for (int i = 0; i <= 10; ++i){
            load_byte(x[i]);
            __delay_ms(100);
    }
    
	return 0;	// robot is stopped, will continue on next push
}

char go_button_handler(char val){
    if (val == 1){
        // robot was running
        val = 0;
    }

    else if (val == 0){
        // robot was stopped
        val = 1;
    }
    
    return val;
}

