//
//	asciidisplay.c
//
//

void ascii_ctrl_bit_set(unsigned char x)
{
    *GPIO_E_ODR_LOW |= (B_SELECT | x);

}

void ascii_ctrl_bit_clear(unsigned char x)
{
    *GPIO_E_ODR_LOW = B_SELECT | (*GPIO_ODR_LOW & ~x);
}

void ascii_write_controller(unsigned char byte){
    delay_250ns();
    ascii_ctrl_bit_set(B_E);
    *GPIO_E_ODR_HIGH = byte;
    ascii_ctrl_bit_clear(B_E);
    delay_250ns();
}
unsigned char ascii_read_controller(void)
{    
    unsigned char c;
    ascii_ctrl_bit_set(B_E);
    delay_250ns();
    delay_250ns();
    c = *GPIO_E_IDR_HIGH;
    ascii_ctrl_bit_clear(B_E);
    return c;
    
}

void ascii_write_cmd(unsigned char command)
{
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_controller(command);
}

void ascii_write_data(unsigned char data)
{
    ascii_ctrl_bit_set(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_controller(data);
}

unsigned char ascii_read_status(void)
{
    unsigned char c;
	*GPIO_E_MODER = 0x00005555;
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_set(B_RW);
    c = ascii_read_controller();
    *GPIO_E_MODER = 0x55555555;
    return c;
}

unsigned char ascii_read_data(void)
{
    unsigned char c;
    *GPIO_E_MODER = 0x00005555;
    ascii_ctrl_bit_set(B_RS);
    ascii_ctrl_bit_set(B_RW);
    c= ascii_read_controller();
    *GPIO_E_MODER = 0x55555555;
    return c;
    
}


void init_app()
{
    *GPIO_E_MODER = 0x55555555;
}

void ascii_init(void)
{
    while ((ascii_read_status() & 0x80) == 0x80) {} /* vänta tills display är klar för att ta emout kommando */
    delay_mikro(8); /* latenstid för kommando */
    ascii_write_cmd(1); /* Clear Display */
    delay_milli(2); /* istället för 1,53ms */
    
    // function set
    while ((ascii_read_status() & 0x80) == 0x80) {} /* vänta tills display är klar för att ta emout kommando */
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_cmd(0b00111000);
    delay_mikro(39);
    
    // display control
    while ((ascii_read_status() & 0x80) == 0x80) {} /* vänta tills display är klar för att ta emout kommando */
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_cmd(0b00001110);
    delay_mikro(39);
    
    // entry mode set
    while ((ascii_read_status() & 0x80) == 0x80) {} /* vänta tills display är klar för att ta emout kommando */
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_write_cmd(0b00000110); //bestämmer MARKÖREN - om det inte funkar testa "ascii_write_cmd(0b00000100);"
    delay_mikro(39);
}

void ascii_gotoxy(int row, int column)
{
    int address = row - 1;
    if(column == 2) address += 0x40;

    ascii_write_cmd(0x80 | address);
}

void ascii_write_char(unsigned char c)
{
    while ((ascii_read_status() & 0x80) == 0x80) {} /* vänta tills display är klar för att ta emout kommando */
    delay_mikro(8);
    ascii_write_data(c);
    delay_mikro(43);
}


int main(int argc, char **argv)
{
    char *s;
    char test1[] = "Player 1: ";
    char test2[] = "Player 2: ";
    
    init_app();
    ascii_init();
    ascii_gotoxy(1,1);
    s = test1;
    while(*s)
    {
		ascii_write_char(*s++);
    }
    
    ascii_gotoxy(1,2);
    s = test2;
    while(*s)
    {
		ascii_write_char(*s++);
    }
    return 0;
}



