

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

void move_object(POBJECT o)
{
	clear_object(o);
	// Bestäm ny position genom att addera riktningskoordinater till akutuell position
	o->posx += o->dirx;
	o->posy += o->diry;
	// varje if-sats ser till att bollen håller sig innanför displayen
		
	if(o->posx < 1){
	// skifta x-riktning 180 grader	
		o->dirx *= (-1);
	}
	
	// längst åt höger, därför vill vi ta 128 - strl så att inte bollen försvinner ut
	if(o->posx > (128 - o->geo->sizex)){
	// skifta x-riktning 180 grader
		o->dirx *= (-1);
	}
	
	if(o->posy < 1){
	// skifta y-riktning 180 grader
		o->diry *= (-1);
	}
	
	 // längst upp på displayen därav - strl
	if(o->posy > (64 - o->geo->sizey)){
	// skifta y-riktning 180 grader
		o->diry *= (-1);
	}

	draw_object(o);
		
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

<<<<<<< HEAD
/* SPELAR KONFIGURATIONER */ 
char player1score = 0;
char player2score = 0;

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
=======
static OBJECT player1bar = 
{
	&bar_geometry,		// geometri för en  boll
	0,0,				// initiala riktningskoordinater
	0,0,				// initial startposition
	draw_object,
	clear_object,
	move_object,
	set_object_speed
};

static OBJECT player2bar = 
{
	&bar_geometry,		// geometri för en  boll
	0,0,				// initiala riktningskoordinater
	128,0,				// initial startposition
	draw_object,
	clear_object,
	move_object,
>>>>>>> a01e037b0541b730007e1fa8b3571f80c57d34cf
	set_object_speed
};