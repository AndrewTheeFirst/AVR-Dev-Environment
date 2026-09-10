#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include <avr/delay.h>

#define BAUD 9600

void uart_init(void){
    uint16_t ubrr = (F_CPU / 16 / BAUD) - 1;

    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0B = (1 << TXEN0);

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_char(char c){
    while (!(UCSR0A & (1 << UDRE0))){}

    UDR0 = c;
}

void uart_send_string(const char *str){
    while (*str){
        uart_send_char(*str++);
    }
}

int main(void){
    uart_init();

    while (1){
        uart_send_string("Hello World!\r\n");
        _delay_ms(200);
    }
}