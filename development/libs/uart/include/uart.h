#ifndef BAUD
    #error BAUD not defined. Try 9600
#endif

#ifndef F_CPU
    #error F_CPU not defined. 16000000UL
#endif

void uart_init(void);

void uart_send_char(char c);

void uart_send_string(const char *str);