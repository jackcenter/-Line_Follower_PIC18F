/*
 * File:   motors.c
 * Author: Jack
 *
 * Created on November 27, 2020, 3:29 PM
 */


#include <xc.h>
#include <stdlib.h>
#include <pic18f87k22.h>
#include <motors.h>

#define _XTAL_FREQ 16000000

#define STBY LATGbits.LG0
#define AIN1 LATGbits.LG1
#define AIN2 LATGbits.LG2
#define PWMA LATGbits.LG3
#define PWMB LATGbits.LG4
#define BIN2 LATFbits.LATF2
#define BIN1 LATFbits.LATF1

void init_motors(){
    TRISF = 0;
    TRISG = 0;              // PORTG pins are all outputs
    T2CON = 0b00000001;     // Off, PS = 4
    CCPTMRS1 = 0b00000000;  // CCP4/5 based on TMR2 for PWM
    
    PR2 = 99;               // TMR2 Period Register for 10kHz
    
    CCP4CON = 0b00001100;   // PWM mode
    CCPR4L = 0;             // 0% duty cycle

    CCP5CON = 0b00001100;   // PWM mode
    CCPR5L = 0;           	// 0% duty cycle    

    T2CONbits.TMR2ON = 1;   // Start the timer
    
	STBY = 0;

    AIN1 = 0;
    AIN2 = 1;
    
    BIN1 = 1;
    BIN2 = 0;
}


void set_duty_cycle(char side, signed char duty_cycle){
		
	if (side == 'r'){

		if (duty_cycle >= 0){
			AIN1 = 0;
			AIN2 = 1;
		}

		else {
			AIN1 = 1;
			AIN2 = 0;
		}

		CCPR4L = abs(duty_cycle);
	}

	else if (side == 'l'){
		
		if (duty_cycle >= 0){
			BIN1 = 1;
			BIN2 = 0;
		}

		else {
			BIN1 = 0;
			BIN2 = 1;
		}

		CCPR5L = abs(duty_cycle);
	}

}

void motors_brake(){
	AIN1 = 0;
	AIN2 = 0;
	BIN1 = 0;
	BIN2 = 0;
}



void motors_drive(signed char dc_right, signed char dc_left){
	set_duty_cycle('r', dc_right);
 	set_duty_cycle('l', dc_left);
}	

void motors_engage(){
    STBY = 1;
}

void motors_disengage(){
    STBY = 0;
}

void motors_turn_around(){
    motors_drive(25, -25);
    __delay_ms(3000);
}

void motors_test(){
       motors_drive(25, 25);
        __delay_ms(1000);
        
        motors_brake();
        __delay_ms(1000);
        
        motors_drive(-50, -50);
        __delay_ms(1000);
        
        motors_brake();
        __delay_ms(1000);
        
        motors_drive(-25, 75);
        __delay_ms(1000);
        
        motors_brake();
        __delay_ms(1000);
}
