#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BAUD 9600UL

#define SEND_NAME_DEMO_1 0
#define BUTTON_DEMO 0
#define GET_SERIAL_MSG_DEMO 0
#define SEND_NAME_DEMO_2 0
#define SIMPLE_CALCULATOR_DEMO 1

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
}

static FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void init_uart(void){
    UBRR0 = GET_UART_UBRR0(BAUD); 
    UCSR0C = UART_CONF_C_8_BIT;
    UCSR0B = UART_CONF_B_ENABLE_TX | UART_CONF_B_ENABLE_RX;
    stdout = &uart_output;
}

void SendSerialMsg(char* string){
    while(*string != '\0'){
        if(UART_TX_REG_EMPTY){
            UDR0 = *string;
            string++;
        }
    }
}

#define UNREAD_DATA (UCSR0A & (1 << RXC0)) 

void GetSerialMsg(char char_buffer[]){
    uint8_t index = 0;
    while (1){
        while (UNREAD_DATA){
            char character = UDR0;
            if (character){
                char_buffer[index++] = character;
                if (character == '\n'){
                    char_buffer[index] = '\0';
#if GET_SERIAL_MSG_DEMO
                    SendSerialMsg(buffer); // verify functionality
                    index = 0;
#else
                    return;
#endif
                } 
            }
        }
    }
}

void init_led(void){
    DDRD =  0b11111111; // enable all leds as output
    PORTD = 0; // enable all leds as output
}

void init_button(void){
    PORTC = 1 << PORTC2; // Enable PULLUP

    // Enable Interrupt
    PCICR = 1 << PCIE1;
    PCMSK1 = 1 << PCINT10;
    sei();
}

void send_name_demo1(void){
    char* string = "Andrew\n";
    while (1){
        SendSerialMsg(string);
        _delay_ms(250);
    }
}

void send_name_demo2(void){
    char* string = "Andrew";
    uint8_t count = 0;
    while (1){
        printf("%s: %d\n", string, count++);
        _delay_ms(500);
    }
}

void calculator_demo(void){
    char num1[3], num2[3];
    int8_t num1_val, num2_val;
    char op[1];
    while (1){
        printf("Enter Number 1: ");
        GetSerialMsg(num1);
        num1_val = atoi(num1);
        printf("%d\n", num1_val);

        printf("Enter Number 2: ");
        GetSerialMsg(num2);
        num2_val = atoi(num2);
        printf("%d\n", num2_val);
        
        printf("Enter Number Operation (1 = +, 2 = -): ");
        GetSerialMsg(op);
        printf("%d\n", atoi(op));

        switch (atoi(op)){
            case 1: // +
                printf("%d + %d = %d\n", num1_val, num2_val, num1_val + num2_val);
                break;
            case 2:
                printf("%d - %d = %d\n", num1_val, num2_val, num1_val - num2_val);
                break; // -
            default:
                printf("Invalid Operator\n");
                break;
        }
        printf("\n");
    }
}

int main(void){
    init_uart();
#if SEND_NAME_DEMO_1
    send_name_demo1();
#elif BUTTON_DEMO
    init_led();
    init_button();
#elif GET_SERIAL_MSG_DEMO
    char buffer[64];
    GetSerialMsg(buffer);
#elif SEND_NAME_DEMO_2
    send_name_demo2();
#elif SIMPLE_CALCULATOR_DEMO
    calculator_demo();
#else
    while(1){}
#endif
    return 0;
}

#define BUTTON_PRESSED (~PINC & (1 << PINC2))

ISR(PCINT1_vect){
    if (BUTTON_PRESSED){
        PORTD |= 1 << DDD4;
        SendSerialMsg("LED ON\n");
    }
    else{
        PORTD &= ~(1 << DDD4);
        SendSerialMsg("LED OFF\n");
    }
}