#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TOP_FOR_20_MS  5000
#define TOP_FOR_1_MS   250

#define IDEAL_SERVO
#define SURVEY 1

#ifdef IDEAL_SERVO
#define TOP_FOR_180_DEG     TOP_FOR_1_MS + 250
#define TOP_FOR_157_5_DEG   TOP_FOR_1_MS + 219
#define TOP_FOR_135_DEG     TOP_FOR_1_MS + 188
#define TOP_FOR_112_5_DEG   TOP_FOR_1_MS + 156
#define TOP_FOR_90_DEG      TOP_FOR_1_MS + 125
#define TOP_FOR_67_5_DEG    TOP_FOR_1_MS + 94
#define TOP_FOR_45_DEG      TOP_FOR_1_MS + 63
#define TOP_FOR_22_5_DEG    TOP_FOR_1_MS + 31
#define TOP_FOR_0_DEG       TOP_FOR_1_MS + 0
#else
#define TOP_FOR_180_DEG     550
#define TOP_FOR_157_5_DEG   498
#define TOP_FOR_135_DEG     445
#define TOP_FOR_112_5_DEG   393
#define TOP_FOR_90_DEG      340
#define TOP_FOR_67_5_DEG    288
#define TOP_FOR_45_DEG      235
#define TOP_FOR_22_5_DEG    183
#define TOP_FOR_0_DEG       130
#endif

#define T1_CONFIG_A_FAST_PWM_ADV    ((1 << WGM11) | (1 << WGM10))
#define T1_CONFIG_A_INVERTING       ((1 << COM1B1) | (1 << COM1B0))
#define T1_CONFIG_A_NON_INVERTING   (1 << COM1B1)
#define T1_CONFIG_B_FAST_PWM_ADV    ((1 << WGM13) | (1 << WGM12))
#define T1_CONFIG_B_CS_64           ((1 << CS11) | (1 << CS10))

void start_pwm(void){
    OCR1A = TOP_FOR_20_MS;
    OCR1B = TOP_FOR_0_DEG;
    DDRB = (1 << DDB2); // Set PWM pin to OUTPUT
    TCCR1A = T1_CONFIG_A_FAST_PWM_ADV | T1_CONFIG_A_INVERTING;
    TCCR1B = T1_CONFIG_B_FAST_PWM_ADV | T1_CONFIG_B_CS_64;
}

int main(void){
    start_pwm();
    int16_t index = 250;
    const uint16_t degs[] = {
        TOP_FOR_180_DEG,
        TOP_FOR_157_5_DEG,
        TOP_FOR_135_DEG,
        TOP_FOR_112_5_DEG,
        TOP_FOR_90_DEG,
        TOP_FOR_67_5_DEG,
        TOP_FOR_45_DEG,
        TOP_FOR_22_5_DEG,
        TOP_FOR_0_DEG
    };
    int8_t dir = 2;

#if SURVEY
    while(1){
        index += dir;
        if (index >= TOP_FOR_180_DEG || index <= TOP_FOR_0_DEG ){
            dir = -dir;
        }
        OCR1B = index;
        _delay_ms(26);
    }
#else
    while(1){
        index = (index + 1) % 9;
        OCR1B = degs[index];
        _delay_ms(1000);
    }
#endif

}