#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <GL/gl.h>
#include <FTGL/ftgl.h>
#include <stdarg.h>
#include "space.h"

#define NO_SPACE 1
#define NO_CASH 2
#define NOT_AVIL 3
#define NO_FUEL 4

//set DEBUG to 1 for debuggin infomation
#define DEBUG 1

SDL_Surface *screen;

int current_screen = 0;

//used for testing
int game_move = 0;

int screen_x = 1024;
int screen_y = 768;

static FTGLfont *font;

static char const* menu_items[]= {
    "Status","Ship Info","Planet Info","Buy Cargo","Sell Cargo","Star Map"
};





// recreate  printf as a GL thing
void glPrintf(float x,float y, char const * fmt, ...) {
    static char buf[1024];
    va_list vl;

    va_start(vl, fmt);
    vsnprintf(buf, sizeof(buf), fmt, vl);
    va_end(vl);

    glRasterPos2f(x,y);
    ftglRenderFont(font, buf, FTGL_RENDER_ALL);
}









void draw_background(void) {

    glClearColor(0,0,0,0);
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

    size_t num_items= sizeof(menu_items)/sizeof(*menu_items);
    float spacing = 1.0 / num_items;

    for (size_t item=0; item<num_items; item++)
    {
        glVertex2f((item*spacing),0);
        glVertex2f((item*spacing),0.1);
    }

    glEnd();

    /* Set the font size and render a small text. */
    ftglSetFontFaceSize(font, 25, 12);

    for (size_t item=0; item<num_items; item++)
    {
        if ((int)item == current_screen)
            glColor4f(1,0,0,1);
        else
            glColor4f(1,1,1,1);

        glPrintf(0.01+(item*spacing),0.09,"%s",menu_items[item]);
    }

}




void draw_status(void) {

    draw_background();

    glPrintf(0.1,0.2,"Commander: %s",player.name);
    glPrintf(0.1,0.3,"Location: %s",planets[player.place].name);
    glPrintf(0.1,0.4,"Worth: %d",player.cash);

// update the screen buffer
    SDL_GL_SwapBuffers();

}





void draw_ship_info(void) {

    draw_background();

    glPrintf(0.1,0.2,"Ship Name: %s",ship.name);
    glPrintf(0.1,0.3,"Avil cargo capacity: %d",ship.cap);
    glPrintf(0.1,0.4,"Range: %d",ship.fuel);

// update the screen buffer
    SDL_GL_SwapBuffers();

}






void draw_planet_info(void) {

    draw_background();

struct planet * p = &planets[player.place];

    glPrintf(0.1,0.2,"Name: %s",p->name);

// update the screen buffer
    SDL_GL_SwapBuffers();

}






void draw_buy_stuff(void) {

    draw_background();

struct planet * p = &planets[player.place];
for (int i=0; i<num_cargo_types; i++)
    {
       glPrintf(0.1,0.3+(0.06*i),"%s:%d:price:%d",cargo_types[i].name,p->cargo_types[i].avil,p->cargo_types[i].price);
    }

// update the screen buffer
    SDL_GL_SwapBuffers();

}






void draw_sell_stuff(void) {

    draw_background();

struct planet * p = &planets[player.place];
struct space_ship * s = player.ship;
for (int i=0; i<num_cargo_types; i++)
    {
        glPrintf(0.1,0.3+(0.06*i),"%s:%d:price:%d",cargo_types[i].name,s->cargo[i].avil,p->cargo_types[i].price);
    }

// update the screen buffer
    SDL_GL_SwapBuffers();

}




void draw_local_map(void) {

    draw_background();

// update the screen buffer
    SDL_GL_SwapBuffers();

}







void draw(void) {
    if ( current_screen == 0 ) {
        draw_status();
    }
    if ( current_screen == 1 ) {
        draw_ship_info();
    }
    if ( current_screen == 2 ) {
        draw_planet_info();
    }
    if ( current_screen == 3 ) {
        draw_buy_stuff();
    }
    if ( current_screen == 4 ) {
        draw_sell_stuff();
    }
    if ( current_screen == 5 ) {
        draw_local_map();
    }
}









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
            printf( "[DEBUG] no fuel to fly to %s\n", planets[dest].name );
        }
        return NO_FUEL;
    }

    dude->place=dest;
    dude->ship->fuel-= distance;
    if (DEBUG) {
        printf ("[DEBUG] flying to:%s\n",planets[dest].name);
    }
    return 0;
}






int buy_fuel( struct player * dude , int quanity) {
dude->ship->fuel += quanity;
dude->cash -= quanity * fuel_cost;
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








int do_game_move(void) {
game_move++;
game_move%=6;

  switch (game_move){
     case 0:
        buy_fuel(&player,20);
        return 0;
     case 1:
        fly_to_planet(&player,1);
        return 0;
     case 2:
        buy_cargo(&player,0,3);
        return 0;
     case 3:
        fly_to_planet(&player,2);
        return 0;
     case 4:
        sell_cargo(&player,0,3); 
        return 0;
     default:
        return 0;
}

}








int mouse_handler( int mouse_x, int mouse_y ) {
size_t num_items= sizeof(menu_items)/sizeof(*menu_items);
if (mouse_y < (0.1 * screen_y))
{
current_screen= num_items * mouse_x / screen_x;
}
return 0;
}








int keyboard_handler( SDLKey keyPressed ) {
 switch (keyPressed)
    {
       case SDLK_ESCAPE:
            return 0;
       case SDLK_LEFT:
            current_screen+=5;
            current_screen%=6;
            break;
       case SDLK_RIGHT:
            current_screen++;
            current_screen%=6;
            break;
       case SDLK_UP:      
            do_game_move();
            break;
       default:
            break;
      }
 return 0;
}










int main(void) {

    /* Create a pixmap font from a TrueType file. */
    font = ftglCreatePixmapFont("FreeSans.ttf");
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(screen_x, screen_y, 0, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE);
    SDL_WM_SetCaption("Simple Window", "Simple Window");
    atexit(SDL_Quit);

    for(;;) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }

            if (event.type == SDL_VIDEORESIZE) {
screen = SDL_SetVideoMode(event.resize.w,event.resize.h, 0, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE);
            glViewport(0,0,event.resize.w,event.resize.h);
            screen_x=event.resize.w;
            screen_y=event.resize.h;
}

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                mouse_handler( event.button.x, event.button.y );
            }

            if (event.type == SDL_KEYDOWN) {
                keyboard_handler( event.key.keysym.sym );
            }
        }
        draw();
    }
    return 0;
}





