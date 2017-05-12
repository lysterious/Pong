/*
 * 	startup.c
 *
 */
#include "delay.c"
#include "asciidisplay.h"
#include "definitions.h"
#include "objects.h"
#include "keypad.c"

void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );

void startup ( void )
{
asm volatile(
	" LDR R0,=0x2001C000\n"		/* set stack */
	" MOV SP,R0\n"
	" BL main\n"				/* call main */
	".L1: B .L1\n"				/* never return */
	) ;
}


void init_app(void)
{
	*GPIO_E_MODER = 0x55555555; //15-0 utgångar
	
	*GPIO_D_MODER = 0x55005555; // V KEYB V 
    *GPIO_D_OTYPER &= 0x00FF;
    *GPIO_D_OTYPER |= 0x0000;
    *GPIO_D_PUPDR &= 0x0000FFFF;
    *GPIO_D_PUPDR |= 0x00aa0000;
}

void main(void)
{
	unsigned char c;
	POBJECT b = &ball;
	POBJECT p1 = &player1;
	POBJECT p2 = &player2;
	init_app();
	graphic_initalize();
	graphic_clear_screen();

	while(1)
	{
		p->move(p);
		delay_milli(40);
		c = keyb();
		*DGPIO_ODR_LOW = c;
		switch(c)
		{
			case 6:	p->set_speed(p, 2, 0); break;
			case 4:	p->set_speed(p, -2, 0); break;
			case 2:	p->set_speed(p, 0, -2); break;
			case 8:	p->set_speed(p, 0, 2); break;
		}
	}
}

