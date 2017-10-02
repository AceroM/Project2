
#ifndef STATE_H
#define STATE_H

#include <cstdlib>
#include <iostream>
#include <vector>

#include "bot.h"
#include "outstream.h"

enum tile {EMPTY, BLOCKED, SHIP, DMG};

struct state {
  // dimensions
  int rows;
  int cols;
  // map
  std::vector <std::vector<tile> > map;

  int initial_ships;
  int ships;
  int round;

  // UI
  bool play;
  bool alive;
};

void init(state &s, int rows, int cols, int smallest, int largest, int number);

void update(state &s, Screen &screen, std::ostream &gamelog);

#endif
