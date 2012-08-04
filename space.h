#pragma once

#define num_cargo_types 2



struct location 
{
int x;
int y;
};


struct cargo
{
char const * name;
int base_price;
int contraband;
};





struct econ
{
int price;
int avil;
};









//setup of the cargo types and prices.
struct cargo cargo_types[]={
{.name="Water",
.base_price=50,
.contraband=0,
},

{.name="Food",
.base_price=100,
.contraband=0,
},

{.name="Furs",
.base_price=300,
.contraband=0,
},

{.name="Ore",
.base_price=400,
.contraband=0,
},

{.name="Games",
.base_price=200,
.contraband=0,
},

{.name="Firearms",
.base_price=900,
.contraband=1,
},

{.name="Medicine",
.base_price=500,
.contraband=0,
},

{.name="Machines",
.base_price=1100,
.contraband=0,
},

{.name="Narcotics",
.base_price=3000,
.contraband=1,
},

{.name="Robots",
.base_price=5000,
.contraband=0,
},
};







struct space_ship
{
char const * name;
int type;
int cap;
struct econ cargo [num_cargo_types];
int num_cargo;
};









struct player
{
char const * name;
int cash;
struct space_ship * ship;
};









struct planet
{
char const * name;
int techlevel;
int govt;
char const * notes;
int police;
int pirates;
struct location pos;
struct econ cargo_types [num_cargo_types];
};

