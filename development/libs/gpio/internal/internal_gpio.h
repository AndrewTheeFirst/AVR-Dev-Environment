#pragma once
#include "gpio.h"

#define NUM_GROUP_PINS 8

struct gpio_pin {
    volatile uint8_t* _ddr;
    volatile uint8_t* _port;
    volatile uint8_t* _pin;
    enum gpio_pin_group _pin_group;
    uint8_t pin_num;
};