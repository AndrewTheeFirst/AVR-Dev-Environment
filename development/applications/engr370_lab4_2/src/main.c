#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SMOOTH_DELAY_MS 26
#define DELAY_1_MS 1000
#define IDEAL_SERVO 0

#define BUTTON_CONTROLLED 1
#define CHECK_DEGS 0
#define SCAN 0

#define TOP_FOR_20_MS  5000
#define TOP_FOR_1_MS   250

#if IDEAL_SERVO
#define TOP_FOR_180_DEG     (TOP_FOR_1_MS + 250)
#define TOP_FOR_157_5_DEG   (TOP_FOR_1_MS + 219)
#define TOP_FOR_135_DEG     (TOP_FOR_1_MS + 188)
#define TOP_FOR_112_5_DEG   (TOP_FOR_1_MS + 156)
#define TOP_FOR_90_DEG      (TOP_FOR_1_MS + 125)
#define TOP_FOR_67_5_DEG    (TOP_FOR_1_MS + 94)
#define TOP_FOR_45_DEG      (TOP_FOR_1_MS + 63)
#define TOP_FOR_22_5_DEG    (TOP_FOR_1_MS + 31)
#define TOP_FOR_0_DEG       (TOP_FOR_1_MS + 0)
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
#define T1_CONFIG_B_FAST_PWM_ADV    ((1 << WGM13) | (1 << WGM12))
#define T1_CONFIG_B_CS_64           ((1 << CS11) | (1 << CS10))

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

void start_pwm(void){
    OCR1A = TOP_FOR_20_MS;
    OCR1B = TOP_FOR_0_DEG;
    DDRB = (1 << DDB2); // Set PWM pin to OUTPUT
    TCCR1A = T1_CONFIG_A_FAST_PWM_ADV | T1_CONFIG_A_INVERTING;
    TCCR1B = T1_CONFIG_B_FAST_PWM_ADV | T1_CONFIG_B_CS_64;
}

void init_switches(void){
    DDRC &= ~((1 << DDC2) | (1 << DDC1) | (1 << DDC0));    // Set switches as INPUT
    PORTC |= (1 << PORTC2) | (1 << PORTC1) | (1 << PORTC0); // Enable switch PULLUPs
    
    // Enable switch interrupts
    PCMSK1 = (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10);
    PCICR = 1 << PCIE1;
    sei();
}

void button_control(void){
    init_switches();
    while(1){}
}

void check_degs(void){
    uint8_t index = 0;
    while(1){
        OCR1B = degs[index];
        index = (index + 1) % 9;
        _delay_ms(DELAY_1_MS);
    }
}

void scan(void){
    while (1){
        for (int16_t index = TOP_FOR_180_DEG; index > TOP_FOR_0_DEG; index -= (TOP_FOR_180_DEG - TOP_FOR_0_DEG) / 32){
            OCR1B = index;
            _delay_ms(SMOOTH_DELAY_MS);
        }
        for (int16_t index = TOP_FOR_0_DEG; index < TOP_FOR_180_DEG; index += (TOP_FOR_180_DEG - TOP_FOR_0_DEG) / 32){
            OCR1B = index;
            _delay_ms(SMOOTH_DELAY_MS);
        }
    }
}

int main(void){
    start_pwm();
#if BUTTON_CONTROLLED
    button_control();
#elif CHECK_DEGS
    check_degs();
#elif SCAN
    scan();
#endif
    return 0;
}

#define BUTTON_1_PRESSED (1 << PINC2)
#define BUTTON_2_PRESSED (1 << PINC1)
#define BUTTON_3_PRESSED (1 << PINC0)

ISR(PCINT1_vect){
    uint8_t switches = ~PINC & ((1 << PINC1) | (1 << PINC2) | (1 << PINC0));
    switch(switches){
        case BUTTON_3_PRESSED | BUTTON_2_PRESSED | BUTTON_1_PRESSED:
            OCR1B = TOP_FOR_22_5_DEG;
            break;
        case BUTTON_3_PRESSED | BUTTON_2_PRESSED:
            OCR1B = TOP_FOR_45_DEG;
            break;
        case BUTTON_3_PRESSED | BUTTON_1_PRESSED:
            OCR1B = TOP_FOR_67_5_DEG;
            break;
        case BUTTON_2_PRESSED | BUTTON_1_PRESSED:
            OCR1B = TOP_FOR_135_DEG;
            break;
        case BUTTON_1_PRESSED:
            OCR1B = TOP_FOR_180_DEG;
            break;
        case BUTTON_2_PRESSED:
            OCR1B = TOP_FOR_157_5_DEG;
            break;
        case BUTTON_3_PRESSED:
            OCR1B = TOP_FOR_112_5_DEG;
            break;
        default:
            OCR1B = TOP_FOR_90_DEG;
            break;
    }
}
