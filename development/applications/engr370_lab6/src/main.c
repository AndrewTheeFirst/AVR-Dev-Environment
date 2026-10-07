#define F_CPU 16000000UL
#define BAUD 9600UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define CIRC_BUFF_TEST 0
#define CIRC_BUFF_DEMO 0
#define QUEUED_SERIAL_MSG_DEMO 1

#define UART_CONF_C_8_BIT ((1 << UCSZ01) | (1 << UCSZ00))
#define UART_CONF_B_ENABLE_TX ((1 << TXEN0))
#define UART_CONF_B_ENABLE_RX ((1 << RXEN0))
#define GET_UART_UBRR0(baud) ((F_CPU / (16 * baud)) - 1)

#define UART_TX_REG_EMPTY (UCSR0A & (1 << UDRE0))

static int uart_putchar(char c, FILE* stream){
    while(1){
        if(UART_TX_REG_EMPTY){
            UDR0 = c;
            break;
        }
    }
    return 0;
}

static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void init_uart(void){
    UBRR0 = GET_UART_UBRR0(BAUD); 
    UCSR0C |= UART_CONF_C_8_BIT;
    UCSR0B |= UART_CONF_B_ENABLE_TX | UART_CONF_B_ENABLE_RX;
    stdout = &uart_output;
    
}

#define UART_QUEUE_SIZE 64

volatile char uart_circ_queue[UART_QUEUE_SIZE];
volatile bool uart_queue_full = false;
volatile uint8_t uart_queue_head, uart_queue_tail;

int uart_push_char(char c){
    if (uart_queue_full){ // queue is full
        return EXIT_FAILURE;
    }
    uart_circ_queue[uart_queue_head] = c;
    uart_queue_head = (uart_queue_head + 1) & (UART_QUEUE_SIZE - 1);
    if (uart_queue_head == uart_queue_tail){
        uart_queue_full = true;
    }
    return EXIT_SUCCESS;
}

int uart_pop_char(char* c){
    if ((!uart_queue_full) && (uart_queue_head == uart_queue_tail)){ // queue is empty; nothing to pop
        return EXIT_FAILURE;
    }
    *c = uart_circ_queue[uart_queue_tail];
    uart_queue_tail = (uart_queue_tail + 1) & (UART_QUEUE_SIZE - 1);
    uart_queue_full = false;
    return EXIT_SUCCESS;
}


#define UNREAD_DATA (UCSR0A & (1 << RXC0))

void init_led(void){
    DDRD =  0b11111111; // enable all leds as output
}

void init_button(void){
    PORTC |= (1 << PORTC2) | (1 << PORTC1); // Enable PULLUP

    // Enable Interrupt
    PCICR = 1 << PCIE1;
    PCMSK1 |= (1 << PCINT10) | (1 << PCINT9);
    sei();
}

void read_from_queue(void){
    while (1){
        uint8_t c;
        if (uart_pop_char(&c) == EXIT_SUCCESS){
            printf("popped off: %d\n", c);
            continue;
        }
        printf("Empty\n");
    }
}

int main(void){
    init_uart();
    init_button();
#if QUEUED_SERIAL_MSG_DEMO
    init_led();
    while(1){}
#elif CIRC_BUFF_DEMO
    read_from_queue();
#else
    while(1){}
#endif
    return EXIT_SUCCESS;
}

void _start_uart_transmission(void){
    UCSR0B |= (1 << UDRIE0);
}

void _stop_uart_transmission(void){
    UCSR0B &= ~(1 << UDRIE0);
}

int SendSerialMsg(char* string){
    while(*string != '\0'){
        if(uart_push_char(*string) == EXIT_FAILURE){
            break;
        }
        string++;
    }
    _start_uart_transmission();
    return EXIT_SUCCESS;
}

#define BUTTON_1_PRESSED (~PINC & (1 << PINC2))
#define BUTTON_2_PRESSED (~PINC & (1 << PINC1))

ISR(PCINT1_vect){
#if CIRC_BUFF_TEST
    if (BUTTON_1_PRESSED){
        if (uart_push_char('a') == EXIT_FAILURE){
            printf("Buffer is Full.\n");
            return;
        }
        printf("Successfully added 'a' to buffer.\n");
    }
    if (BUTTON_2_PRESSED){
        char c;
        if (uart_pop_char(&c) == EXIT_FAILURE){
            printf("Buffer is Empty.\n");
            return;
        }
        printf("Successfully removed '%c' from buffer.\n", c);
    }
#elif CIRC_BUFF_DEMO
    if (BUTTON_1_PRESSED){
        for (uint8_t index = 1; index <= 10; index++){
            if (uart_push_char(index) == EXIT_FAILURE){
                printf("Buffer is Full.\n");
                return;
            }
            printf("Added: %d\n", index);
        }
    }
    else{ // released
        for (uint8_t index = 11; index <= 20; index++){
            if (uart_push_char(index) == EXIT_FAILURE){
                printf("Buffer is Full.\n");
                return;
            }
            printf("Added: %d\n", index);
        } 
    }
#elif QUEUED_SERIAL_MSG_DEMO
    if (BUTTON_1_PRESSED){
            PORTD |= 1 << DDD4;
            SendSerialMsg("LED ON\n");
        }
    else{
            PORTD &= ~(1 << DDD4);
            SendSerialMsg("LED OFF\n");
        }
#endif
}

ISR(USART_UDRE_vect){
    char c;
    if (uart_pop_char(&c) == EXIT_SUCCESS){
        UDR0 = c;
    }
    else{
        _stop_uart_transmission();
    }
}