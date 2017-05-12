/*
 * 	startup.c
 *
 */
#include "delay.h"
#include "definitions.h"
#include "keypad.h"

//#define SIMULATOR

void startup(void) __attribute__((naked)) __attribute__((section (".start_section")) );

void startup ( void )
{
asm volatile(
	" LDR R0,=0x2001C000\n"    	/* set stack */
	" MOV SP,R0\n"
	" BL main\n"            	/* call main */
	".L1: B .L1\n"            	/* never return */
	) ;
}



//välj grafik-display och ettställ de bitar som är 1 i x
void graphic_ctrl_bit_set (uint8_t x)
{
	 *GPIO_E_ODR_LOW |= x;
}

//välj grafik-display och nollställ de bitar som är 1 i x
void graphic_ctrl_bit_clear (uint8_t x)
{
	*GPIO_E_ODR_LOW &= ~x;
}

// konfigurera CS-signalerna
void select_controller (uint8_t controller)
{
    
	switch (controller)
	{
		case 0:
			graphic_ctrl_bit_clear(B_CS1);
        	graphic_ctrl_bit_clear(B_CS2);
        	break;
       	 
    	case B_CS1:
        	graphic_ctrl_bit_set(B_CS1);
        	graphic_ctrl_bit_clear(B_CS2);
        	break;
   	 
    	case B_CS2:
        	graphic_ctrl_bit_set(B_CS2);
        	graphic_ctrl_bit_clear(B_CS1);
        	break;
   		 
		case B_CS1|B_CS2:
        	graphic_ctrl_bit_set(B_CS2);
        	graphic_ctrl_bit_set(B_CS1);
        	break;
       	 

// WHAT THE ACTUAL FUCK!?!?!?! TWO CASES IN ONE CHAR?!?!?! PROBLEM SOLVED MY GOOD FRIEND ;))
   	 
	}
}

void graphic_wait_ready (void)
{
    char input;
	graphic_ctrl_bit_clear(B_E);
	*GPIO_E_MODER = 0x00005555; //15-8 ingångar, 7-0 utgångar
    
	graphic_ctrl_bit_clear(B_RS);
	graphic_ctrl_bit_set(B_RW);
    
	delay_500ns();
    
	while (1)
	{
    	graphic_ctrl_bit_set(B_E);
    	delay_500ns();
		input = *GPIO_E_IDR_HIGH & LCD_BUSY;
    	graphic_ctrl_bit_clear(B_E);
    	delay_500ns();
    	//om den är noll (=klar) så hoppar vi ur while-satsen
   	 // WHAT THE ACTUAL FUCKFUCK!=!?!??!
    	if (input == 0 )
    	{
        	break;
    	}
	}
    
	graphic_ctrl_bit_set(B_E);
	*GPIO_E_MODER = 0x55555555; //15-0 utgångar
}

uint8_t graphic_read (uint8_t controller)
{    
	graphic_ctrl_bit_clear(B_E);
	*GPIO_E_MODER = 0x00005555; //15-8 ingångar, 7-0 utgångar
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_set(B_RW);
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	uint8_t rv = *GPIO_E_IDR_HIGH;
	graphic_ctrl_bit_clear(B_E);
	*GPIO_E_MODER = 0x55555555; // 15-0 utgångar
    
	if (controller == B_CS1) {
    	select_controller(B_CS1);
    	graphic_wait_ready();
	}
	if (controller == B_CS2) {
    	select_controller(B_CS2);
    	graphic_wait_ready();
	}
	// det måste vara display_read = rv ?
	return rv;
}

void graphic_write(uint8_t value, uint8_t controller)
{
	// temp
	*GPIO_E_MODER = 0x55555555; // 15-0 utgångar
    
    
	*GPIO_E_ODR_HIGH = value;
	select_controller(controller);
	delay_500ns();
	graphic_ctrl_bit_set(B_E);
	delay_500ns();
	graphic_ctrl_bit_clear(B_E);
    
	//controller kommer alltid vara på?
	if(controller & B_CS1){
    	select_controller(B_CS1);
    	graphic_wait_ready();
	}
	if(controller & B_CS2){
    	select_controller(B_CS2);
    	graphic_wait_ready();
	}
	*GPIO_E_ODR_HIGH = 0;
	graphic_ctrl_bit_set(B_E);
	select_controller(0);
}

void graphic_write_command (uint8_t command, uint8_t controller)
{
	graphic_ctrl_bit_clear(B_E);
	select_controller(controller);
	graphic_ctrl_bit_clear(B_RS);
	graphic_ctrl_bit_clear(B_RW);
	graphic_write(command, controller);
}

void graphic_write_data (uint8_t data, uint8_t controller)
{
	graphic_ctrl_bit_clear(B_E);
	select_controller(controller);
	graphic_ctrl_bit_set(B_RS);
	graphic_ctrl_bit_clear(B_RW);
	graphic_write(data, controller);
}

uint8_t graphic_read_data(uint8_t controller)
{
	(void) graphic_read(controller);
	return graphic_read(controller);
}

void graphic_initalize(void)
{
	// TEMP Sätter utporten till utgångar. Osäker om det sker i init.
	*GPIO_E_MODER = 0x55555555;
	// TEMP
	graphic_ctrl_bit_set(B_E);
	delay_mikro(10);
	graphic_ctrl_bit_clear(B_CS1);
	graphic_ctrl_bit_clear(B_CS2);
	graphic_ctrl_bit_clear(B_RESET);
	graphic_ctrl_bit_clear(B_E);
    
	delay_milli(30);
    
	graphic_ctrl_bit_set(B_RESET);
    
	graphic_write_command(LCD_OFF,     	(B_CS1|B_CS2));
	graphic_write_command(LCD_ON,         	(B_CS1|B_CS2));
	graphic_write_command(LCD_DISP_START, 	(B_CS1|B_CS2));
	graphic_write_command(LCD_SET_ADD, 	(B_CS1|B_CS2));
	graphic_write_command(LCD_SET_PAGE, 	(B_CS1|B_CS2));
    
	select_controller(0);   	 
}

void graphic_clear_screen(void)
{
	for(int page = 0; page < 8; page++) {
    	graphic_write_command( (LCD_SET_PAGE | page),	(B_CS1|B_CS2));
    	graphic_write_command( (LCD_SET_ADD | 0),     	(B_CS1|B_CS2));
    	for(int add = 0; add < 64; add++) {
        	graphic_write_data(0, (B_CS1|B_CS2));
    	}
	}
}


void pixel (unsigned int x, unsigned int y, unsigned char set)
{
	// för linjer i MD407
	// delay_milli(10); 
	
	uint8_t mask, temp, controller;
	int index;
	
	if ((x < 1) || (y < 1) || (x > 128) || (y > 64)) return;
	
	//skapa en bitmask för pixeln
	index = (y-1) / 8;
	
	switch ((y-1) % 8)
	{
		case 0: mask = 1; break;
		case 1: mask = 2; break;
		case 2: mask = 4; break;
		case 3: mask = 8; break;
		case 4: mask = 0x10; break;
		case 5: mask = 0x20; break;
		case 6: mask = 0x40; break;
		case 7: mask = 0x80; break;
	}
	// om pixel inte ska fyllas i
	if (set == 0) mask = ~mask;
	
	// bestäm fysiska koordinater och välj styrkrets
	if (x > 64) {
		controller = B_CS2;
		x = x - 65;
	} else {
		controller = B_CS1;
		x= x - 1;
	}
	
	graphic_write_command(LCD_SET_ADD | x, controller);
	graphic_write_command(LCD_SET_PAGE | index, controller);
	temp = graphic_read_data(controller);
	graphic_write_command(LCD_SET_ADD | x, controller);
	if (set) {
		//om pixel INTE ska fyllas i
		mask |= temp;
	} else {
		//om pixel ska fyllas i
		mask &= temp;
	}
	graphic_write_data(mask, controller);
}

















