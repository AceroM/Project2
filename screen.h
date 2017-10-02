
#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include "common.h"

struct symbol { 
  char ch;
  color clr;
};

class Screen {
  std::vector< std::vector <symbol> > symbols; 
  bool in_range(int row, int col);

public: 
  Screen(int rows, int cols);
  int get_rows ();
  int get_cols ();
  void mark(int row, int col, char ch, color clr = GRAY);
  char read(int row, int col);
  color read_color(int row, int col);
};

#endif
