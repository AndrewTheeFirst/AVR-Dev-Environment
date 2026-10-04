#include "linkedlist.h"

struct gpio_isr_handle{
    gpio_isr gp_isr;
    enum interrupt_signal signal;
    volatile struct gpio_pin* gp_pin;
    struct inode node;
};