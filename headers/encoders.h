/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef ENCODERS_H
#define	ENCODERS_H

#include <xc.h> 

struct Encoder 
{
    char pin_A;
    char pin_B;
    char reading;
    char reading_last;
    char reading_next;
    int count;
};

struct Encoder init_encoder(char, char);
void start_encoders(void);
void stop_encoders(void);

#endif

