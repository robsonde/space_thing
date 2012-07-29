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







