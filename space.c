#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "space.h"

#define NO_SPACE 1
#define NO_CASH 2
#define NOT_AVIL 3



//setup of the planets.
struct planet planets[]={
{
.name="bob",
.notes="planet bob",
.cargo_types={ {.price=100,.avil=50} }, 
.pos={.x=1,.y=1},
},

{
.name="foo",
.notes="planet foo",
.cargo_types={ {.price=175,.avil=5} }, 
.pos={.x=2,.y=2},
},

{
.name="flarg",
.notes="planet flarg",
.cargo_types={ {.price=125,.avil=0},{.price=100,.avil=1} },
.pos={.x=3,.y=3},
},

};








// setup of a ship.
struct space_ship ship={
.name = "Dragon",
.cap=14,
};






// setup of the player.
struct player player={
.name="the dude",
.cash=1000,
.ship=&ship,
};





//prints basic player stats.
void print_player(struct player * p)
{
printf("%s\n","Player stats");
printf("  Commander:%s\n",p->name);
printf("  Worth:%d\n\n",p->cash);
}




//print the infomation of a ship.
void print_ship(struct space_ship * s)
{
printf("%s\n","Ship stats");
printf("  Ship Name:%s\n",s->name);
for (int i=0;i<num_cargo_types;i++)
{
printf("  %s:%d\n",cargo_types[i].name,s->cargo[i].avil);  
} 
printf("  Avil capacity:%d\n",s->cap);  
printf("\n"); 
};
 



//prints a given planet.
void print_planet (struct planet * p)
{
printf("%s\n","planet stats");
printf("  %s\n",p->name);
for (int i=0;i<num_cargo_types;i++)
{
printf("  %s:%d:price:%d\n",cargo_types[i].name,p->cargo_types[i].avil,p->cargo_types[i].price);  
}  
printf("\n"); 
}







//function for buying cargo from a planet. 
int buy_cargo ( struct player * dude , struct planet * planet,int cargo_index, int count ){
	int cost = count*planet->cargo_types[cargo_index].price;

	//checking if we have cash and space for this trade.
	if ( dude->ship->cap < count ) return NO_SPACE;
	if ( dude->cash < cost ) return NO_CASH;
	if ( planet->cargo_types[cargo_index].avil < count ) return NOT_AVIL;

	//all looks good so we do the trade.
	planet->cargo_types[cargo_index].avil -= count;
	dude->cash -= cost;
	dude->ship->cargo[cargo_index].avil += count;
	dude->ship->cap -= count;
	return 0;
}




//function for selling cargo to a planet.
int sell_cargo ( struct player * dude , struct planet * planet,int cargo_index, int count  ){
	
	//check that we have what we are selling.
	if ( dude->ship->cargo[cargo_index].avil < count ) return NOT_AVIL;

	//all looks good so we do the trade.
	planet->cargo_types[cargo_index].avil += count; 
	dude->cash += count*planet->cargo_types[cargo_index].price;
	dude->ship->cargo[cargo_index].avil -= count;
	dude->ship->cap += count;
	return 0;
}






void init (void)
{
glClearColor(0,0,0,0);
}


void display (void)
{
glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
glutSwapBuffers ();
}


void reshape (int Width,int Height)
{
glViewport (0,0,Width,Height);
}

void tick (void)
{



glutPostRedisplay();
}








// this is only a main for testing other functions and data setup.
int main (void)
{
print_planet(&planets[0]);
print_ship(&ship);
print_player(&player);
printf("-------------------\n");

buy_cargo(&player,&planets[0],0,7);
buy_cargo(&player,&planets[2],1,1);

print_planet(&planets[0]);
print_ship(&ship);
print_player(&player);
printf("-------------------\n");

sell_cargo(&player,&planets[1],0,7);

print_planet(&planets[0]);
print_ship(&ship);
print_player(&player);
printf("-------------------\n");

return 0;
}





/*int main(int argc, char** argv)
{
  // GLUT Window Initialization:
  glutInit (&argc, argv);
  glutInitWindowSize (640, 480);
  glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow ("Space Thing");

  // Initialize OpenGL graphics state
  init();

  // Register callbacks:
  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  //glutKeyboardFunc (Keyboard);
  //glutMouseFunc (MouseButton);
  //glutMotionFunc (MouseMotion);
  glutIdleFunc (tick);

  // Turn the flow of control over to GLUT
  glutMainLoop ();
  return 0;
}
*/

