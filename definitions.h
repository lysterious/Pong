//
//	definitions.h
//
//

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define GPIO_E 0x40021000
#define GPIO_E_MODER    	((volatile unsigned int*)     	(GPIO_E))
#define GPIO_E_OTYPER 		((volatile unsigned short*) 	(GPIO_E+0x4))
#define GPIO_E_OSPEEDER	((volatile unsigned int* )    	(GPIO_E+0x8))
#define GPIO_E_PUPDR    	((volatile unsigned int*)     	(GPIO_E+0xC))
#define GPIO_E_IDR_LOW		((volatile unsigned char*) 	(GPIO_E+0x10))
#define GPIO_E_IDR_HIGH	((volatile unsigned char*) 	(GPIO_E+0x11))
#define GPIO_E_ODR_LOW		((volatile unsigned char*) 	(GPIO_E+0x14))
#define GPIO_E_ODR_HIGH	((volatile unsigned char*) 	(GPIO_E+0x15))

#define GPIO_D 			0x40020C00
#define GPIO_D_MODER    	((volatile unsigned int*)     	(GPIO_D))
#define GPIO_D_OTYPER 		((volatile unsigned short*) 	(GPIO_D+0x4))
#define GPIO_D_OSPEEDER	((volatile unsigned int* )    	(GPIO_D+0x8))
#define GPIO_D_PUPDR    	((volatile unsigned int*)     	(GPIO_D+0xC))
#define GPIO_D_IDR_LOW		((volatile unsigned char*) 	(GPIO_D+0x10))
#define GPIO_D_IDR_HIGH	((volatile unsigned char*) 	(GPIO_D+0x11))
#define GPIO_D_ODR_LOW		((volatile unsigned char*) 	(GPIO_D+0x14))
#define GPIO_D_ODR_HIGH	((volatile unsigned char*) 	(GPIO_D+0x15))


#define	B_E       	0x40
#define	B_RESET    	0x20
#define	B_CS2    	0x10
#define	B_CS1   	0x8
#define	B_SELECT   	0x4
#define	B_RW   		0x2
#define	B_RS   		0x1
 
#define	LCD_ON        	0x3F
#define	LCD_OFF        0x3E
#define	LCD_SET_ADD 	0x40
#define	LCD_SET_PAGE	0xB8
#define	LCD_DISP_START	0xC0
#define	LCD_BUSY    	0x80

#define MAX_POINTS	20

// STRUCTS
typedef unsigned char uint8_t;

typedef struct tPoint {
	unsigned char x;
	unsigned char y;
} POINT;
 
typedef struct tGeometry {
	int		numpoints;
	int		sizex;
	int		sizey;
	POINT 	px[ MAX_POINTS ];
} GEOMETRY, *PGEOMETRY;

typedef struct tObj {
	PGEOMETRY 	geo;
	int 		dirx, diry;
	int 		posx, posy;
	void		(* draw ) (struct tObj *);
	void		(* clear ) (struct tObj *);
	void		(* move ) (struct tObj *);
	void		(* set_speed ) (struct tObj *, int, int);
}	OBJECT, *POBJECT;

GEOMETRY ball_geometry =
{
	12,		// numpoints
	4,4, 	// sizex, sizey
	{
		// px[0,1,2...]
		  {0,1},{0,2}, 
	{1,0},{1,1},{1,2},{1,3}, 
	{2,0},{2,1},{2,2},{2,3}, 
		  {3,1},{3,2},
	}
};

GEOMETRY bar_geometry =
{
	10, // numpoints
	1, 10, // sizex, sizey
	{ 
		// px[0,1,2,3,4...]
	{0,0},
	{1,0},
	{2,0},
	{3,0},
	{4,0},		
	{5,0},
	{6,0},
	{7,0},
	{8,0},
	{9,0},
	}
};
#endif