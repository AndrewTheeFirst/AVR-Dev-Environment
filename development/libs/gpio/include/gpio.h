#pragma once
#include <stdint.h>

#define LL_HIGH 1
#define LL_LOW 0

struct gpio_pin;

enum gpio_pin_group{
    GROUP_B,
    GROUP_C,
    GROUP_D,
};

enum gpio_config {
    GPIO_CONFIG_OUTPUT,
    GPIO_CONFIG_INPUT,
    GPIO_CONFIG_IPULLUP
};

struct gpio_pin* gpio_init(enum gpio_pin_group pin_group, uint8_t pin_num);

void gpio_config(struct gpio_pin* gp_pin, enum gpio_config gp_config);

void gpio_set_level(struct gpio_pin* gp_pin, uint8_t logic_level);

void gpio_toggle_level(struct gpio_pin* gp_pin);

uint8_t gpio_read_level(struct gpio_pin* gp_pin);