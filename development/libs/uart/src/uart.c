#include "uart.h"
#include <avr/io.h>
#include <stdint.h>

void uart_init(void){
    uint16_t ubrr = (F_CPU / 16 / BAUD) - 1;

    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0B = (1 << TXEN0); // Enable Transmitter

    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-N-1 Mode
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