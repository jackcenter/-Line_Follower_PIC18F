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
void set_motor_duty_cycle(char, signed char);
void motors_brake(void);
void motors_drive(signed char, signed char);
void motors_engage(void);
void motors_disengage(void);
void motors_turn_around(void);
void motors_test(void);

#endif

