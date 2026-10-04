#include <avr/interrupt.h>
#include "interrupts.h"
#include "internal_gpio.h"
#include <stddef.h>
#include "uart.h"

static volatile uint8_t portb_prev_states = 0;
static volatile uint8_t portc_prev_states = 0;
static volatile uint8_t portd_prev_states = 0;

#define PREV_PINB_LL(pin) (portb_prev_states & (1 << pin))
#define PREV_PINC_LL(pin) (portc_prev_states & (1 << pin))
#define PREV_PIND_LL(pin) (portd_prev_states & (1 << pin))

static volatile struct inode portb_isr_inf_head = {0};
static volatile struct inode portc_isr_inf_head = {0};
static volatile struct inode portd_isr_inf_head = {0};

static bool module_is_init = false;

void init_module(void){
    init_list(&portb_isr_inf_head);
    init_list(&portc_isr_inf_head);
    init_list(&portd_isr_inf_head);

    portb_prev_states = PINB;
    portc_prev_states = PINC;
    portd_prev_states = PIND;

    module_is_init = true;
}

void gpio_register_isr(struct gpio_isr_handle* isr_handle, volatile struct gpio_pin* gp_pin, gpio_isr gp_isr, enum interrupt_signal signal){
    isr_handle->gp_isr = gp_isr;
    isr_handle->gp_pin = gp_pin;
    isr_handle->signal = signal;
    uint8_t pin_num = gp_pin->pin_num;
    switch (gp_pin->_pin_group){
        case GROUP_B:
            link_inode(&portb_isr_inf_head, &isr_handle->node);
            break;
        case GROUP_C:
            link_inode(&portc_isr_inf_head, &isr_handle->node);
            break;
        case GROUP_D:
            link_inode(&portd_isr_inf_head, &isr_handle->node);
            break;
    }
}

void gpio_unregister_isr(struct gpio_isr_handle* isr_handle){
    unlink_inode(&isr_handle->node);
}

void gpio_enable_int(volatile struct gpio_pin* gp_pin){
    switch (gp_pin->_pin_group){
        case GROUP_B:
            PCICR |= 1 << PCIE0; // activate pin-group interrupts
            PCMSK0 |= (1 << gp_pin->pin_num);
            break;
        case GROUP_C:
            PCICR |= 1 << PCIE1;
            PCMSK1 |= (1 << gp_pin->pin_num);
            break;
        case GROUP_D:
            PCICR |= 1 << PCIE2;
            PCMSK2 |= (1 << gp_pin->pin_num);
            break;
    }
}

void gpio_disable_int(volatile struct gpio_pin* gp_pin){
    switch (gp_pin->_pin_group){
        case GROUP_B:
            PCMSK0 &= ~(1 << gp_pin->pin_num);
            break;
        case GROUP_C:
            PCMSK1 &= ~(1 << gp_pin->pin_num);
            break;
        case GROUP_D:
            PCMSK2 &= ~(1 << gp_pin->pin_num);
            break;
    }
}

#define MAKE_PCISR(pcvector, pcmask, isr_inf_head, prev_pin_ll)                                         \
    ISR(pcvector){                                                                                      \
        for(struct inode* current_node = (&isr_inf_head)->right;                                        \
        current_node != &isr_inf_head;                                                                  \
        current_node = current_node->right){                                                            \
            struct gpio_isr_handle* handle = CONTAINER_OF(current_node, struct gpio_isr_handle, node);  \
            uint8_t pindex = handle->gp_pin->pin_num;                                                   \
            struct gpio_pin* gp_pin = handle->gp_pin;                                                   \
            if (pcmask & (1 << pindex) && handle->gp_isr){                                              \
                switch(handle->signal){                                                                 \
                    case SIGNAL_EDGE_RISING:                                                            \
                        uart_send_string("rising!\n");\
                        if (!prev_pin_ll(pindex) && gpio_read_level(gp_pin)){                           \
                            handle->gp_isr(handle);                                                     \
                        }                                                                               \
                        break;                                                                          \
                    case SIGNAL_EDGE_FALLING:                                                           \
                        uart_send_string("falling!\n");\
                        if (prev_pin_ll(pindex) && !gpio_read_level(gp_pin)){                           \
                            handle->gp_isr(handle);                                                     \
                        }                                                                               \
                        break;                                                                          \
                    case SIGNAL_EDGE_RISING_AND_FALLING:                                                \
                        uart_send_string("rising and falling!\n");\
                        if (prev_pin_ll(pindex) != gpio_read_level(gp_pin)){                            \
                            handle->gp_isr(handle);                                                     \
                        }                                                                               \
                        break;                                                                          \
                }                                                                                       \
            }                                                                                           \
        }                                                                                               \
    }

MAKE_PCISR(PCINT0_vect, PCMSK0, portb_isr_inf_head, PREV_PINB_LL)
MAKE_PCISR(PCINT1_vect, PCMSK1, portc_isr_inf_head, PREV_PINC_LL)
MAKE_PCISR(PCINT2_vect, PCMSK2, portd_isr_inf_head, PREV_PIND_LL)