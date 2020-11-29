/*
 * File:   motors.c
 * Author: Jack
 *
 * Created on November 27, 2020, 3:29 PM
 */


#include <xc.h>
#include <pic18f87k22.h>
#include <motors.h>

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
    CCPR4L = 50;             // 50 duty cycle

    CCP5CON = 0b00001100;   // PWM mode
    CCPR5L = 50;           // 50% duty cycle    

    T2CONbits.TMR2ON = 1;   // Start the timer
    
    AIN1 = 0;
    AIN2 = 1;
    STBY = 1;
    
    BIN1 = 1;
    BIN2 = 0;
}