#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "gpio.h"

#define NUM_GPIOS 8
#define DELAY_MS 75

enum leds_state {
    LEDS_STATE_STOP,
    LEDS_STATE_STROBE_LEFT,
    LEDS_STATE_STROBE_RIGHT
};

volatile struct gpio_pin* sw1;
volatile struct gpio_pin* sw2;
volatile struct gpio_pin* sw3;

volatile enum leds_state curr_leds_state = LEDS_STATE_STOP;

ISR(PCINT1_vect){
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

void strobe_controller(void){

    PCICR = 1 << PCIE1; // activate pin-group interrupts
    PCMSK1 = (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2); // activate specific pin-group pins' interrupt
    sei(); // activate global interrupts

    // initialize buttons
    sw1 = gpio_init(GROUP_C, 1);
    gpio_config(sw1, GPIO_CONFIG_IPULLUP);

    sw2 = gpio_init(GROUP_C, 2);
    gpio_config(sw2, GPIO_CONFIG_IPULLUP);

    sw3 = gpio_init(GROUP_C, 0);
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
    uint8_t led_index = 0;
    while (1){
        struct gpio_pin* gp_pin = gp_pins[led_index];
        if (curr_leds_state != LEDS_STATE_STOP){
            gpio_set_level(gp_pin, LL_LOW);
            if (curr_leds_state == LEDS_STATE_STROBE_RIGHT){
                if (++led_index >= NUM_GPIOS){ led_index = 0; }           
            }
            else if (curr_leds_state == LEDS_STATE_STROBE_LEFT){
                if (led_index-- == 0){ led_index = NUM_GPIOS - 1; }
            }
            gp_pin = gp_pins[led_index];
        }
        gpio_set_level(gp_pin, LL_HIGH);
        _delay_ms(DELAY_MS);
    }
}

int main(void){
    strobe_controller();
    return 0;
}