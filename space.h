#pragma once

#define num_cargo_types 10
#define num_planets 30
#define fuel_cost 5


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






struct cargo cargo_types[num_cargo_types];







struct space_ship
{
char const * name;
int type;
int cap;
struct econ cargo [num_cargo_types];
int num_cargo;
int fuel;
};









struct player
{
char const * name;
int cash;
struct space_ship * ship;
int place;
};









struct planet
{
char const * name;
int techlevel;
int govt;
char const * notes;
int visited;
int police;
int pirates;
struct location pos;
struct econ cargo_types [num_cargo_types];
};






extern struct planet planets[num_planets];

struct space_ship ship;

struct player player;

