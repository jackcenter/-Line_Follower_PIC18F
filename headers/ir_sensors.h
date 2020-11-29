#ifndef IR_SENSORS_H
#define	IR_SENSORS_H

#include <xc.h> 

struct IRSensor
{
    char adcon0_value;
    char index;
    char led;
    char position;
    struct IRSensor *next_sensor;
};

void init_ADC(struct IRSensor *);
void start_ADC(void);
void stop_ADC(void);
short read_and_update_ADC(struct IRSensor *);
char convert_measurement_to_binary(short, short);

#endif

