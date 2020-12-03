/*
 * File:   main.c
 * Author: Jack
 *
 * Created on November 17, 2020, 7:58 PM
 * 
 * TMR1 - main.c, PS 8
 * TMR2 - motors.c, PS 4
 *
 * CCP2 - TMR1, observer
 * CCP3 - TMR1, control
 * CCP4 - TMR2, motors.h
 * CCP5 - TMR2, motors.h
 * CCP6 - TMR1, shift_register.h - display update
 * CCP7 - TMR1, go_button.h - debounce
 */

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic18f87k22.h>
#include <shift_register.h>
#include <go_button.h>
#include <ir_sensors.h>
#include <motors.h>
#include <encoders.h>

#define _XTAL_FREQ 16000000
#pragma config FOSC=HS1, PWRTEN=ON, BOREN=ON, BORV=2, PLLCFG=OFF
#pragma config WDTEN=OFF, CCP2MX=PORTC, XINST=OFF

#define IR0 0b00000001
#define IR1 0b00000101
#define IR2 0b00001001
#define IR3 0b00001101
#define IR4 0b00010001
#define READINGS_MAX 2
#define SENSORS_MAX 4
#define ADC_CUTOFF 3200
#define OBSERVE 5000        // ps8 instructions for 10ms
#define CONTROL 50000       // ps8 instructions for 100ms
#define DISPLAY 10000       // ps8 instructions for 20ms
#define DEBOUNCE 10000      // ps8 instructions for 20ms



// PORT B
#define ENC_1A 5
#define ENC_1B 4

char go_flag = 0;
char go_flag_0 = 0;
char button_state = 0;
char button_state_0 = 0;

char adc_flag = 0;
short adc_reading = 0;

struct IRSensor IR_1 = {0b00000101, 0, 6, 1, 0};
struct IRSensor IR_2 = {0b00001001, 1, 5, 0, 0};
struct IRSensor IR_3 = {0b00001101, 2, 4, -1, 0};

// const char adc_sensors[] = {IR1, IR2, IR3};
struct IRSensor *sensor_read = &IR_1;
struct IRSensor *sensor_next = &IR_1;

struct Encoder encoder_A; 
char encoder_readings_old = 0;

char display_value = 1;

void init(void);
void run_sleep_routine(void);
char process_measurement(const short, const char);
char update_sensor(char);
void update_encoders(void);

void main(void) {
	/* TODO: load byte on tmr
	   TODO: update IR array after all sensors have been read
	   (consider taking average of several measurments
	   Read encoders and store values
       Read IR array and store values	
	*/

    init();
	
    char adc_reading_number = 0;
    char display_val = 0b11111110;
    
    while(1){
       
/*        motors_drive(25, 25);
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
*/
        if (go_flag != go_flag_0){
            if (go_flag == 1){
                execute_delivery();
            }
            
            else if (go_flag == 0){
                pause_delivery();
            }
            
            go_flag_0 = go_flag;
        }
        
        if (adc_flag != 0){
            adc_reading_number += 1;
            
            if (adc_reading_number != 1){
                display_val = process_measurement(adc_reading, display_value);
//                load_byte(display_val); // should have this happen on tmr
                adc_reading_number = update_sensor(adc_reading_number);
            }
                  
            adc_flag = 0;
        }
    }
}

void init(){
    OSCCONbits.IDLEN = 0;
    
    // TMR1
    T1CON = 0b00110101;             // On, PS8
    
    // display update
    CCP6CON = 0b00001010;           // Compare generates software interrupt
    CCPTMRS1bits.C6TSEL0 = 0;       // CCP6 -> TMR1
    PIR4bits.CCP6IF = 0;            // clear flag
    IPR4bits.CCP6IP = 0;            // low pri
    PIE4bits.CCP6IE = 0;            // enable
    
    // debounce
    CCP7CON = 0b00001010;           // Compare generates software interrupt
    CCPTMRS1bits.C7TSEL1 = 0;       // CCP7 -> TMR1
    CCPTMRS1bits.C7TSEL0 = 0;
    PIR4bits.CCP7IF = 0;            // clear flag
    IPR4bits.CCP7IP = 0;            // low pri
    PIE4bits.CCP7IE = 0;            // enable
    
        
    RCONbits.IPEN = 1;              // Enable priority levels
    INTCONbits.GIEL = 1;            // Enable low-priority interrupts to CPU
    INTCONbits.GIEH = 1;            // Enable all interrupts
    INTCONbits.PEIE = 1;            // Enable external interrupts
    
    init_SPI();
    init_go_button();
    init_ADC(sensor_next);

    encoder_A = init_encoder(ENC_1A, ENC_1B);
    stop_encoders();
    
    init_motors();
    
    IR_1.next_sensor = &IR_2;
    IR_2.next_sensor = &IR_3;
    IR_3.next_sensor = &IR_1; 

//	T1CON = 0b0000101;
    
    for (int i = 0; i < 2; ++i){
        load_byte(0xFF);
        __delay_ms(500);
        load_byte(0x00);
        __delay_ms(500);
    }
    
    go_flag = 0;
    Sleep();

}

//void run_sleep_routine(){
//    const char x[] = {0xFF, 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 
//                      0xFF}; 
//    stop_ADC();     // delays are way off with ADC running
//        for (int i = 0; i<10; ++i){
//            load_byte(x[i]^0xFF);
//            __delay_ms(100);
//        }
//
//        Sleep();
//
//        for (int i = 9; i >= 0; --i){
//            load_byte(x[i]^0xFF);
//            __delay_ms(100);
//        }
//        start_ADC();
//        go_flag = 0;
//}

char process_measurement(const short reading, char display_val){

    char meas = convert_measurement_to_binary(reading, ADC_CUTOFF);
    display_val |= meas << (sensor_read->led);  // set or clear bit
    return display_val;
}

char update_sensor(char reading){  
    
    if (sensor_next != sensor_read){
        // This was the last measurement from read
        sensor_read = sensor_next;
        reading = 0;
    }

    else if (reading == READINGS_MAX){
        // the in progress measurement will be the last one
        sensor_next = sensor_read->next_sensor;
    } 
    
    return reading;
}

void update_encoders(){
    
    static signed char lookup_table[] = {   
                                            0, -1, 1, 0, 
                                            1, 0, 0, -1,
                                            -1, 0, 0, 1, 
                                            0, 1, -1, 0
                                        }; 
    
    char enc_dual = (PORTB & 0xF0) >> 4;
            
    encoder_A.reading = encoder_A.reading << 2;
    char test = (enc_dual & 0b0011);
    encoder_A.reading = encoder_A.reading | test;
    encoder_A.count += lookup_table[encoder_A.reading & 0x0F];
}


/******************************************************************************
 * HiPriISR interrupt service routine
 ******************************************************************************/

void __interrupt() HiPriISR(void) {
    
    while(1) {
        if (PIR1bits.SSP1IF) {
            // SPI is ready
			display_byte();
            PIR1bits.SSP1IF = 0;
            continue;
        }
        
        else if (INTCONbits.INT0IF){
            
            CCPR7L = TMR1L + (char)(DEBOUNCE & 0x00FF);
            CCPR7H = TMR1H + (char)((DEBOUNCE >> 8) & 0x00FF);
            PIR4bits.CCP7IF = 0;
            PIE4bits.CCP7IE = 1;

            INTCONbits.INT0IE = 0; // disable interrupt until debounce complete
            INTCONbits.INT0IF = 0;
            
            button_state_0 = PORTBbits.RB0;
            continue;
        }   
        
        
        break;      
    }
}

/******************************************************************************
 * LoPriISR interrupt service routine
 ******************************************************************************/

void __interrupt(low_priority) LoPriISR(void) 
{
    // Save temp copies of WREG, STATUS and BSR if needed.
    while(1) {
        if( PIR1bits.ADIF){    
			// ADC acquisition finished
            adc_reading = read_and_update_ADC(sensor_next);
            adc_flag = 1;
            PIR1bits.ADIF = 0;              
            continue;
        }
        
        else if (INTCONbits.RBIF){
            // External encoder interrupt detected
            update_encoders();
            INTCONbits.RBIF = 0;
            continue;
        }
        
        else if (PIR4bits.CCP7IF && PIE4bits.CCP7IE){
            // Debounce time is over, don't really need to check if pressed
            button_state = PORTBbits.RB0;
            
            if (button_state && button_state_0){ // both high
                go_flag = go_button_handler(go_flag);
            }
            
            PIE4bits.CCP7IE = 0;        // disable CCP7
            PIR4bits.CCP7IF = 0;
            INTCONbits.INT0IF = 0;
            INTCONbits.INT0IE = 1;      // enable external interrupt
            continue;
            
        }
        // TODO load display byte every 20 ms, including blink
		// Update PID measurments every 10 ms, inputs every 100ms
        break;  
    }
}
