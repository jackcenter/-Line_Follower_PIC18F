/*
 * File:   go_button.c
 * Author: Jack
 *
 * Created on November 18, 2020, 8:15 PM
 */

#include <xc.h>
#include <pic18f87k22.h>
#include <go_button.h>

#define GO_T TRISB0
#define GO_P PORTB0

void init_go_button(){
    TRISBbits.GO_T = 1;
    
    INTCONbits.INT0IF = 0;  // Clear flag
    INTCONbits.INT0IE = 1;  // Enable (always high pri)
}

char execute_delivery(){
    return 1;
}