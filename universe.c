#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "space.h"







//setup of the cargo types and prices.
struct cargo cargo_types[]= {
    {   .name="Water",
        .base_price=50,
        .contraband=0,
    },

    {   .name="Food",
        .base_price=100,
        .contraband=0,
    },

    {   .name="Furs",
        .base_price=300,
        .contraband=0,
    },

    {   .name="Ore",
        .base_price=400,
        .contraband=0,
    },

    {   .name="Games",
        .base_price=200,
        .contraband=0,
    },

    {   .name="Firearms",
        .base_price=900,
        .contraband=1,
    },

    {   .name="Medicine",
        .base_price=500,
        .contraband=0,
    },

    {   .name="Machines",
        .base_price=1100,
        .contraband=0,
    },

    {   .name="Narcotics",
        .base_price=3000,
        .contraband=1,
    },

    {   .name="Robots",
        .base_price=5000,
        .contraband=0,
    },
};





// setup of a ship.
struct space_ship ship= {
    .name = "Dragon",
    .cap=14,
    .fuel=14,
};





// setup of the player.
struct player player= {
    .name="The Dude",
    .cash=1000,
    .ship=&ship,
    .place=0,    //which planet do we start at
};

