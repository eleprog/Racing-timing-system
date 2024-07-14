ldi	r16, 0x00
ldi	r17, 0x01
ldi	r18, 0x50

out	DDRB,	r17; DDRB   = 1
out	PORTB,	r17; PORTB  = 1
out	OSCCAL, r18; OSCCAL = 0x50

start:
	out	PORTB, r16; PORTB = 0;
	nop
	nop
	nop
	nop
	nop
	out	PORTB, r17; PORTB = 1;
	nop
	nop
	nop
rjmp start
