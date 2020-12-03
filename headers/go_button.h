/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef GO_BUTTON_H
#define	GO_BUTTON_H

#include <xc.h>
#include <shift_register.h>
#include <ir_sensors.h>
#include <motors.h>

void init_go_button(void);
void enable_go_button(void);
void disable_go_button(void);
char execute_delivery(void);
char enter_sleep_mode(void);
char pause_delivery(void);
char go_button_handler(char);

#endif	

