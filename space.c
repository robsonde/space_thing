#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "space.h"

#define NO_SPACE 1
#define NO_CASH 2
#define NOT_AVIL 3
#define NO_FUEL 4

//set DEBUG to 1 for debuggin infomation
#define DEBUG 0


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
    for (int i=0; i<num_cargo_types; i++)
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
    for (int i=0; i<num_cargo_types; i++)
    {
        printf("  %s:%d:price:%d\n",cargo_types[i].name,p->cargo_types[i].avil,p->cargo_types[i].price);
    }
    printf("\n");
}






//function to fly to another planet.
int fly_to_planet( struct player * dude , int dest) {

    struct planet from_planet = planets[dude->place];
    struct planet to_planet = planets[dest];

    int dx = to_planet.pos.x - from_planet.pos.x;
    int dy = to_planet.pos.y - from_planet.pos.y;

//check for fuel
    int distance = hypot(dx,dy);

    if ( distance > dude->ship->fuel ) {
        if (DEBUG) {
            printf( "[DEBUG] no fuel to fly to %d\n", dest );
        }
        return NO_FUEL;
    }

    dude->place=dest;
    if (DEBUG) {
        printf ("[DEBUG] flying to:%s\n",planets[dest].name);
    }
    return 0;
}








//function for buying cargo from a planet.
int buy_cargo ( struct player * dude, int cargo_index, int count ) {
    struct planet planet = planets[dude->place];
    int cost = count * planet.cargo_types[cargo_index].price;

    //checking if we have cash and space for this trade.
    if ( dude->ship->cap < count ) {
        if (DEBUG) {
            printf ("[DEBUG] buy cargo: NO_SPACE\n");
        }
        return NO_SPACE;
    }

    if ( dude->cash < cost ) {
        if (DEBUG) {
            printf ("[DEBUG] buy cargo: NO_CASH:%d\n",cost);
        }
        return NO_CASH;
    }

    if ( planet.cargo_types[cargo_index].avil < count ) {
        if (DEBUG) {
            printf ("[DEBUG] buy cargo: NO_AVIL\n");
        }
        return NOT_AVIL;
    }

    //all looks good so we do the trade.
    planet.cargo_types[cargo_index].avil -= count;
    dude->cash -= cost;
    dude->ship->cargo[cargo_index].avil += count;
    dude->ship->cap -= count;
    if ( DEBUG ) {
        printf ("[DEBUG] buy cargo:%d:%d\n",cargo_index,count);
    }
    return 0;
}




//function for selling cargo to a planet.
int sell_cargo ( struct player * dude, int cargo_index, int count  ) {
    struct planet planet = planets[dude->place];

    //check that we have what we are selling.
    if ( dude->ship->cargo[cargo_index].avil < count ) {
        if (DEBUG) {
            printf ("[DEBUG] sell cargo: NOT_AVIL\n");
        }
        return NOT_AVIL;
    }


    //all looks good so we do the trade.
    planet.cargo_types[cargo_index].avil += count;
    dude->cash += count*planet.cargo_types[cargo_index].price;
    dude->ship->cargo[cargo_index].avil -= count;
    dude->ship->cap += count;
    if ( DEBUG ) {
        printf ("[DEBUG] sell cargo:%d:%d\n",cargo_index,count);
    }
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

    buy_cargo(&player,0,7);
    print_planet(&planets[0]);
    print_ship(&ship);
    print_player(&player);
    printf("-------------------\n");


    fly_to_planet(&player,2);


    sell_cargo(&player,0,7);

    print_planet(&planets[1]);
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

