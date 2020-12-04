/*
 * File:   74HC595.c
 * Author: Jack
 *
 * Created on November 17, 2020, 8:01 PM
 */
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f87k22.h>
#include <shift_register.h>

// Port C
#define SDO TRISC5
#define SDI TRISC4
#define SCK TRISC3
#define RCL_T TRISC2
#define RCL_L LATC2

#define BLINK_ON 2          // DISPLAY cylces to stay on
#define BLINK_OFF 18        // DISPLAY cycles to stay off


void init_SPI() {

    TRISCbits.SDO = 0;   
    TRISCbits.SDI = 0;   
    TRISCbits.SCK = 0;
    TRISCbits.RCL_T = 0;
    LATCbits.LATC2 = 1;
    
    SSP1CON1 = 0b00100000;  // Enable SPI in master mode
    SSP1STAT = 0b11000000;  
    
    PIR1bits.SSP1IF = 0;    // Clear serial bit flag
    PIE1bits.SSP1IE = 1;    // Enable interrupts
    IPR1bits.SSP1IP = 1;    // High priority
}


void init_display(){
// display update
    CCP6CON = 0b00001010;           // Compare generates software interrupt
    CCPTMRS1bits.C6TSEL0 = 0;       // CCP6 -> TMR1
    PIR4bits.CCP6IF = 0;            // clear flag
    IPR4bits.CCP6IP = 0;            // low pri
    PIE4bits.CCP6IE = 0;            // enable
}


void load_byte(char val) {
    SSPBUF = val^0xFF;  // inverts so 1 means on and 0 means of
}


void display_byte() {
    LATCbits.RCL_L = 0;
    LATCbits.RCL_L = 1;
}


char blink_handler(char count, char *disp){
    if (count != 0){
        --count;
    }

    else if (*disp & 0b00000001){
        // led is on
        *disp ^= 0b00000001;    // toggle led off
        count = BLINK_OFF;
    }

    else{
        // led is off
        *disp ^= 0b00000001;    // toggle led on
        count = BLINK_ON ;
    }

    return count;
}