#pragma once
#include "gpio.h"

#define CONTAINER_OF (ptr, type, member) (type)((char*)ptr - offsetof(type, member))

struct gpio_isr_handle;

typedef void (*gpio_isr)(volatile struct gpio_isr_handle*);

enum interrupt_signal{
    SIGNAL_EDGE_RISING,
    SIGNAL_EDGE_FALLING,
    SIGNAL_EDGE_RISING_AND_FALLING
};

void init_module(void);

void gpio_register_isr(struct gpio_isr_handle* isr_handle, volatile struct gpio_pin* gp_pin, gpio_isr gp_isr, enum interrupt_signal signal);

void gpio_enable_int(volatile struct gpio_pin* gp_pin);

void gpio_disable_int(volatile struct gpio_pin* gp_pin);

#include "internal_interrupts.h"