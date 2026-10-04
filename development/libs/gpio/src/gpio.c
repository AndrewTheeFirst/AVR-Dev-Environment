#include <avr/io.h>

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "internal_gpio.h"

struct gpio_pin* gpio_init(enum gpio_pin_group pin_group, uint8_t pin_num){
    struct gpio_pin* gp_pin = malloc(sizeof(struct gpio_pin));
    if (pin_num >= NUM_GROUP_PINS){
        return NULL;
    }
    switch (pin_group){
        case GROUP_B:
            gp_pin->_ddr =   &DDRB;
            gp_pin->_port=   &PORTB;
            gp_pin->_pin =   &PINB;
            break;
        case GROUP_C:
            gp_pin->_ddr =   &DDRC;
            gp_pin->_port=   &PORTC;
            gp_pin->_pin =   &PINC;
            break;
        case GROUP_D:
            gp_pin->_ddr =   &DDRD;
            gp_pin->_port=   &PORTD;
            gp_pin->_pin =   &PIND;
            break;
        default:
            return NULL;
    }
    gp_pin->_pin_group = pin_group;
    gp_pin->pin_num = pin_num;
    return gp_pin;
}

void gpio_config(struct gpio_pin* gp_pin, enum gpio_config gp_config){
    switch(gp_config){
        case GPIO_CONFIG_OUTPUT:
            *(gp_pin->_ddr) |= (1 << gp_pin->pin_num);
            break;
        case GPIO_CONFIG_INPUT:
            *(gp_pin->_ddr) &= (uint8_t)~(1 << gp_pin->pin_num);
            *(gp_pin->_port) &= (uint8_t)~(1 << gp_pin->pin_num);
            break;
        case GPIO_CONFIG_IPULLUP:
            *(gp_pin->_ddr) &= (uint8_t)~(1 << gp_pin->pin_num);
            *(gp_pin->_port) |= (1 << gp_pin->pin_num);
            break;
    }
}

void gpio_set_level(struct gpio_pin* gp_pin, uint8_t logic_level){
    if (logic_level){
        *(gp_pin->_port) |= (1 << gp_pin->pin_num);
    }
    else{
        *(gp_pin->_port) &= (uint8_t)~(1 << gp_pin->pin_num);
    }
}

void gpio_toggle_level(struct gpio_pin* gp_pin){
    *(gp_pin->_port) ^= (1 << gp_pin->pin_num);
}

uint8_t gpio_read_level(struct gpio_pin* gp_pin){
    return (*(gp_pin->_pin) & (1 << gp_pin->pin_num));
}