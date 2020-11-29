/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef MOTORS_H
#define	MOTORS_H

#include <xc.h> 

struct Motor 
{
    const char PWM;
    const char IN1;
    const char IN2;
    const char STBY;
};

void init_motors(void);

#endif

