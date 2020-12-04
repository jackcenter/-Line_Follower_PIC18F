/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef SHIFT_REGISTER_H
#define	SHIFT_REGISTER_H

#include <xc.h>

void init_SPI(void);
void init_display(void);
void load_byte(char);
void display_byte(void);
char blink_handler(char, char *);

#endif

