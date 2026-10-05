/*
 * Application1.c
 *
 * Created: 9/2/2026 3:18:40 PM
 * Author : 03dre
 */ 

#include <stdint.h>
#include <avr/io.h>

#define F_CPU 16000000UL

#include <util/delay.h>

#define UNIT 300

#define DOT \
		PORTB |= 1 << PORTB5;\
		 _delay_ms(UNIT);\
		PORTB &= (uint8_t)~(1 << PORTB5);\
		 _delay_ms(UNIT)

#define DASH \
	PORTB |= 1 << PORTB5;\
	_delay_ms(UNIT * 3);\
	PORTB &= (uint8_t)~(1 << PORTB5);\
	_delay_ms(UNIT)

void letter_to_morse(char letter){
	switch (letter) {
		case 'a':
	    case 'A':
			DOT;DASH;
			break;
	    case 'b':
	    case 'B':
			DASH;DOT;DOT;DOT;
			break;
	    case 'c':
	    case 'C':
			DASH;DOT;DASH;DOT;
			break;
	    case 'd':
	    case 'D':
			DASH;DOT;DOT;
			break;
	    case 'e':
	    case 'E':
			DOT;
	    	break;
	    case 'f':
	    case 'F':
			DOT;DOT;DASH;DOT;
	    	break;
	    case 'g':
	    case 'G':
			DASH;DASH;DOT;
	    	break;
	    case 'h':
	    case 'H':
			DOT;DOT;DOT;DOT;
	    	break;
	    case 'i':
	    case 'I':
			DOT;DOT;
	    	break;
	    case 'j':
	    case 'J':
			DOT;DASH;DASH;DASH;
	   		break;
	    case 'k':
	    case 'K':
			DASH;DOT;DASH;
	    	break;
	    case 'l':
	    case 'L':
			DOT;DASH;DOT;DOT;
	    	break;
	    case 'm':
	    case 'M':
			DASH;DASH;
	    	break;
	    case 'n':
	    case 'N':
			DASH;DOT;
	    	break;
	    case 'o':
	    case 'O':
			DASH;DASH;DASH;
	    	break;
	    case 'p':
	    case 'P':
			DOT;DASH;DASH;DOT;
	    	break;
	    case 'q':
	    case 'Q':
			DASH;DASH;DOT;DASH;
	    	break;
	    case 'r':
	    case 'R':
			DOT;DASH;DOT;
	    	break;
	    case 's':
	    case 'S':
			DOT;DOT;DOT;
	    	break;
	    case 't':
	    case 'T':
			DASH;
	    	break;
	    case 'u':
	    case 'U':
			DOT;DOT;DASH;
	    	break;
	    case 'v':
	    case 'V':
			DOT;DOT;DOT;DASH;
	    	break;
	    case 'w':
	    case 'W':
			DOT;DASH;DASH;
	    	break;
	    case 'x':
	    case 'X':
			DASH;DOT;DOT;DASH;
	    	break;
	    case 'y':
	    case 'Y':
			DASH;DOT;DASH;DASH;
	    	break;
	    case 'z':
	    case 'Z':
			DASH;DASH;DOT;DOT;
	    	break;
		case ' ':
			_delay_ms(UNIT * 6);
	}
}

void string_to_morse(char* string){
	char letter = *string;
	letter_to_morse(letter);
	string++;
	while(letter != '\0'){
		letter = *string;
		_delay_ms(UNIT*2);
		letter_to_morse(letter);
		string++;
	}
}

int main(void){
	while (1){
		DDRB |= 1 << DDB5; // Set PB5 to OUTPUT
		char name[] = "Andrew ";
		string_to_morse(name);
	}
}