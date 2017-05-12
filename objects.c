// 
//	objects.c
//
char out_of_bounds_x(POBJECT o){
	return ((o->posx)<0 || (o->posx)>(128-(o->geo->sizex))); 
}

char out_of_bounds_y(POBJECT o){
	return ((o->posy)<0 || (o->posy) >(64-(o->geo->sizey))); 
}
void set_object_speed(POBJECT o, int speedx, int speedy) {
	o->dirx = speedx;
	o->diry = speedy;
}

void draw_object(POBJECT o)
{
	// vi är tvugna att först "gå in" i o sen geo! dvs: o->geo-> ..
	for (int i = 0; i < (o->geo->numpoints); i++) {
		pixel(o->posx + o->geo->px[i].x, o->posy + o->geo->px[i].y, 1);
	}
}


void clear_object(POBJECT o)
{
	// vi är tvugna att först "gå in" i o sen geo! dvs: o->geo-> ..
	for (int i = 0; i < (o->geo->numpoints); i++) {
		pixel(o->posx + o->geo->px[i].x, o->posy + o->geo->px[i].y, 0);
	}
}

/* BOLL KONFIGURATIONER */
 void move_ball(POBJECT o)
 {
	 clear_object(o);
	 o->posx = o->posx + o->dirx;
	 o->posy = o->posy + o->diry;
	 
	 if(out_of_bounds_x(o))
		 o->dirx = o->dirx (*-1);

	if(is_out_of_bounds_y(o) || object_collide(o))
		obj->diry = obj->diry * (-1);
	
 }

/* Kollar ifall bollen kolliderar med en spelare */ 
char object_collide(POBJECT o)
{
	// x och y värden till bollen
	int x = o->posx;
	int y = o->posy;
	
	// storleken till objeket
	int sizex = o->geo->sizex;
	int sizey = o->geo->sizey;
	
	//x och y värden till spelare1
	int player1x = ptrplayer1->posx;
	int player1y = ptrplayer1->posy;
	
	// x och y värden till spelare2
	int player2x = ptrplayer2->posx;
	int player2y = ptrplayer2->posy;
	
	// bredden och höjden på spelar-objektet
	int playerheight = o->geo->sizey;
	int playerwidth = o->geo->sizex;
	
	// ifall bollen och spelaren kolliderar returnerar vi 1
	if(((sizex + x) >= player2x) && (y>=player2y) && (y <= player2y + playerheight) && ((y+sizey)>=player2y) && ((y+sizey)<=(player2y+playerheight)))
		return 1;
		
	// ifall bollen och spelaren kolliderar returnerar vi 1	
	if((x <= player1x + playerwidth)) && (y>=player1y) && (y <=(player1y + playerheight)) && ((y + sizey) <= player1y) && ((y + sizey) <= player1y + playerheight) 
		return 1;
		
	// ifall de inte kolliderar så returnerar vi 0
	return 0;
}


static OBJECT ball = 
{
	&ball_geometry,		// geometri för en  boll
	0,0,				// initiala riktningskoordinater
	1,1,				// initial startposition
	draw_object,
	clear_object,
	move_object,
	set_object_speed
};
/* SPELAR KONFIGURATIONER */ 
char player1score = 0;
char player2score = 0;

void move_player(POBJECT o)
{
	if(out_of_bounds_y(o))
		return;
	clear_object(o);
	o->posy = o->posy + o->diry;
	draw_object(o);
}

OBJECT player1 = {
	&player_geometry,
	0,0, // initiala riktningspositioner
	1,1, // initiala startpositioner
	draw_object,
	clear_object,
	move_player,
	set_object_speed
};

OBJECT player2 = {
	&player_geometry,
	0,0, // initiala riktningspositioner
	(128-2),1, // initiala startpositioner
	draw_object,
	clear_object,
	move_player,
	set_object_speed
};

// skapar pointers till våra objekt
static POBJECT ptrball = &ball;
static POBJECT ptrplayer1 = &player1;
static POBJECT ptrplayer2 = &player2;