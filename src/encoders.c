/*
 * File:   encoders.c
 * Author: Jack
 *
 * Created on November 28, 2020, 8:12 AM
 */


#include <xc.h>
#include <pic18f87k22.h>
#include <encoders.h>

struct Encoder init_encoder(char pin_A, char pin_B){
    struct Encoder encoder_new = {pin_A, pin_B, 0, 0, 0, 0};
    
    if (pin_A == 4 || pin_B == 4){
        TRISBbits.TRISB4 = 1;
    }
    
    if (pin_A == 5 || pin_B == 5){
        TRISBbits.TRISB5 = 1;
    }
    
    if (pin_A == 6 || pin_B == 6){
        TRISBbits.TRISB6 = 1;
    }
    
    if (pin_A == 7 || pin_B == 7){
        TRISBbits.TRISB7 = 1;
    } 
    
    // Enable interrupt for specific pins
    INTCONbits.RBIF = 0;        // clear flag
    INTCONbits.RBIE = 1;        // enable Interrupt-on-Change
    INTCON2bits.RBIP = 0;       // low priority
    
    return encoder_new;
}

void start_encoders(){
    INTCONbits.RBIF = 0;        // clear flag
    INTCONbits.RBIE = 1;        // enable Interrupt-on-Change
}

void stop_encoders(){
    INTCONbits.RBIE = 0;        // enable Interrupt-on-Change
    INTCONbits.RBIF = 0;        // clear flag
}

