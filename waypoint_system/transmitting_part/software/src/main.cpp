#define F_CPU 533000

#include <avr/io.h>
#include <avr/interrupt.h>

#define PULSE_FREQ 38000
#define PULSE_AMT 20

#define BLINK	PORTB=1;\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				PORTB=0;\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop")
				
#define DELAY	asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop");\
				asm("nop")

int main(void) {
    DDRB	= 0b00000001;
	PORTB	= 0b00000000;
	OSCCAL	= 0x4D;
    while (1) {
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		BLINK;
		
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;
		DELAY;	
		DELAY;
		DELAY;
		DELAY;
		DELAY;
    }
}