
/*
*     keypad.c
*
*/
#ifndef KEYPAD_H
#define KEYPAD_H

#include <string.h>
#include "definitions.h"

void activateRow(unsigned int row){
    switch(row){
        case 1: *GPIO_D_ODR_HIGH = 0x10;break;
        case 2: *GPIO_D_ODR_HIGH = 0x20;break;
        case 3: *GPIO_D_ODR_HIGH = 0x40;break;
        case 4: *GPIO_D_ODR_HIGH = 0x80;break;
        case 0: *GPIO_D_ODR_HIGH = 0x00;break;
        default: return;
        }
}

int getCol(void){

    unsigned char c;

    c = *GPIO_D_IDR_HIGH;
    if(c & 0x8) return 4;
    if(c & 0x4) return 3;
    if(c & 0x2) return 2;
    if(c & 0x1) return 1;
    return 0;
}

unsigned char key[]={1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};

unsigned char keyb(void){
    int row,col;
        for(row=1; row<=4; row++) {
			activateRow(row);
			if((col = getCol() ) )
				{
				activateRow( 0 );
				return key[4*(row-1)+(col-1)];
			}
		}
    activateRow( 0 );
    unsigned char c = 0xFF;
    return c;

}
#endif