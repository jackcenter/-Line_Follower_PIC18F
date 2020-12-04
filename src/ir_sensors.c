/*
 * File:   ir_sensors.c
 * Author: Jack
 *
 * Created on November 25, 2020, 8:41 PM
 */

#include <xc.h>
#include <ir_sensors.h>

void init_ADC(struct IRSensor *sensor){
    ADCON1 = 0b00110000;    //Configure ADCON1 for AVdd(GND) and AVss(4.096V)
    ADCON2 = 0b10010101;    //Configure ADCON2 for right justified; Tacq = 4Tad 
                            //and Tad = 16Tosc

    ANCON0bits.ANSEL2 = 1;  //Configure AN0 as analog input
    TRISAbits.TRISA2 = 1;
    
    PIR1bits.ADIF = 0;      // Clear
    IPR1bits.ADIP = 0;      // Low priority
    
    ADCON0 = sensor->adcon0_value;
    stop_ADC();
}

void start_ADC(){
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
}

void stop_ADC(){
    PIE1bits.ADIE = 0;
    PIR1bits.ADIF = 0;
    ADCON0bits.ADON = 0;
    ADCON0bits.GO = 0;
    
}

short read_and_update_ADC(struct IRSensor *next_sensor){
    short val = (ADRESH << 8) | ADRESL; //Save low/high values of ADC     
    ADCON0 = next_sensor->adcon0_value; //Configure ADCON0 to read current sensor;
    ADCON0bits.GO = 1;                  //Start acquisition then conversion
    return val;
}

char convert_measurement_to_binary(short val, short max){
    char result = 0;
    
    if (val >= max){
        result = 1;
    }
    
    return result;
}
