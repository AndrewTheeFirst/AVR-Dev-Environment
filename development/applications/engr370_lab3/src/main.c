#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "gpio.h"

#define NUM_GPIOS 8
#define DELAY_MS 75

#define AUTO_COUNTER 0
#define MANUAL_COUNTER 1
#define STROBE_CONTR 2

#define OP_MODE STROBE_CONTR

enum leds_state {
    LEDS_STATE_STOP,
    LEDS_STATE_STROBE_LEFT,
    LEDS_STATE_STROBE_RIGHT
};

void auto_counter(void){
    DDRD = 0b11111111; // enable all as output
    uint8_t count = 0;
    while (1){
        PORTD = count++; // update leds
        _delay_ms(DELAY_MS);
    }
}

void manual_counter(void){
    DDRD =  0b11111111; // enable all led as output
    PORTC = 0b00000111; // enable button pullups 
    uint8_t count = 0;
    while (1){
        if (!(PINC & (1 << PORT1))){ // if sw pressed (active low)
            count++;
        }
        if (!(PINC & (1 << PORT2))){
            count--;
        }
        if (!(PINC & (1 << PORT0))){
            count = 0;
        }
        PORTD = count; // update leds
        _delay_ms(DELAY_MS);
    }
}

void strobe_controller(void){
    // initialize buttons
    struct gpio_pin* sw1 = gpio_init(GROUP_C, 1);
    gpio_config(sw1, GPIO_CONFIG_IPULLUP);

    struct gpio_pin* sw2 = gpio_init(GROUP_C, 2);
    gpio_config(sw2, GPIO_CONFIG_IPULLUP);

    struct gpio_pin* sw3 = gpio_init(GROUP_C, 0);
    gpio_config(sw3, GPIO_CONFIG_IPULLUP);

    // initialize leds
    struct gpio_pin* gp_pins [NUM_GPIOS] = {0};
    for (uint8_t index = 0; index < NUM_GPIOS; index++){
        struct gpio_pin* gp_pin = gpio_init(GROUP_D, index);
        gp_pins[index] = gp_pin;
        // gpio_config(gp_pin, GPIO_CONFIG_OUTPUT); 
    }
    DDRD =  0b11111111; // enable all leds as output

    // mainloop
    enum leds_state curr_leds_state = LEDS_STATE_STROBE_RIGHT;
    uint8_t led_index = 0;
    while (1){
        struct gpio_pin* gp_pin = gp_pins[led_index];
        if (curr_leds_state == LEDS_STATE_STOP){
            gpio_set_level(gp_pin, LL_HIGH);
            _delay_ms(DELAY_MS);
        }
        else{
            gpio_set_level(gp_pin, LL_HIGH);
            _delay_ms(DELAY_MS);
            gpio_set_level(gp_pin, LL_LOW);
            if (curr_leds_state == LEDS_STATE_STROBE_RIGHT){
                if (++led_index >= NUM_GPIOS){ led_index = 0; }           
            }
            else if (curr_leds_state == LEDS_STATE_STROBE_LEFT){
                if (led_index-- == 0){ led_index = NUM_GPIOS - 1; }
            }
        }

        // cute lil state machine
        if (!gpio_read_level(sw1)){      // (buttons active low)
            curr_leds_state = LEDS_STATE_STROBE_RIGHT;
        }
        else if (!gpio_read_level(sw2)){
            curr_leds_state = LEDS_STATE_STROBE_LEFT;
        }
        else if (!gpio_read_level(sw3)){
            curr_leds_state = LEDS_STATE_STOP;
        }
    }
}

int main(void){
    if      (OP_MODE == AUTO_COUNTER)   { auto_counter(); }
    else if (OP_MODE == MANUAL_COUNTER) { manual_counter(); }
    else if (OP_MODE == STROBE_CONTR)   { strobe_controller(); }
    return 0;
}