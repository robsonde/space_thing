#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <GL/gl.h>
#include "space.h"

#define NO_SPACE 1
#define NO_CASH 2
#define NOT_AVIL 3
#define NO_FUEL 4

//set DEBUG to 1 for debuggin infomation
#define DEBUG 0

SDL_Surface *screen;

int current_screen = 0;





void draw_background(void){
glClearColor(1,0,0,0);
glClear(GL_COLOR_BUFFER_BIT);

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0,1,1,0,-1,1);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

glBegin(GL_LINES);
glColor4f(1,1,1,1);
glVertex2f(0,0.1);
glVertex2f(1,0.1);

glVertex2f(0,0);
glVertex2f(0,0.1);

glVertex2f(0.1,0);
glVertex2f(0.1,0.1);

glVertex2f(0.2,0);
glVertex2f(0.2,0.1);

glVertex2f(0.3,0);
glVertex2f(0.3,0.1);

glVertex2f(0.4,0);
glVertex2f(0.4,0.1);

glVertex2f(0.5,0);
glVertex2f(0.5,0.1);

glVertex2f(0.6,0);
glVertex2f(0.6,0.1);

glVertex2f(1,0);
glVertex2f(1,0.1);






glEnd();







}




void draw_status(void) {

draw_background();

// update the screen buffer
    SDL_GL_SwapBuffers();

}





void draw_ship_info(void) {

// fill the screen with black color
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 255, 0));

// update the screen buffer
    SDL_GL_SwapBuffers();

}





void draw_planet_info(void) {

// fill the screen with black color
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 255));

// update the screen buffer
    SDL_GL_SwapBuffers();

}






void draw_buy_stuff(void) {

// fill the screen with black color
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 0));

// update the screen buffer
    SDL_GL_SwapBuffers();

}






void draw_sell_stuff(void) {

// fill the screen with black color
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 0, 255));

// update the screen buffer
    SDL_GL_SwapBuffers();

}




void draw_local_map(void) {

// fill the screen with black color
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 255));

// update the screen buffer
    SDL_GL_SwapBuffers();

}







void draw(void) {
if ( current_screen == 0 ) {draw_status();}
if ( current_screen == 1 ) {draw_ship_info();}
if ( current_screen == 2 ) {draw_planet_info();}
if ( current_screen == 3 ) {draw_buy_stuff();}
if ( current_screen == 4 ) {draw_sell_stuff();}
if ( current_screen == 5 ) {draw_local_map();}
}
















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




int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    screen = SDL_SetVideoMode(1024, 768, 0, SDL_OPENGL|SDL_DOUBLEBUF);

    SDL_WM_SetCaption("Simple Window", "Simple Window");

    atexit(SDL_Quit);

    for(;;) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
            if (event.type ==  SDL_KEYDOWN) {
            current_screen++;
            current_screen%=5;
            }

        }

        draw();
    }


    return 0;
}







