
#include <curses.h>
#include <string>
#include "output.h"
#include "screen.h"

#define X(r,c) (c)*2 + 6
#define Y(r,c) (r)*1 + 3

#define GRAY_COLOR (A_NORMAL | COLOR_PAIR(1))
#define WATER_COLOR (A_NORMAL | COLOR_PAIR(5))

void print_hint(std::string s, char c) {
  bool needs_highlight = true;
  attrset(A_NORMAL | COLOR_PAIR(1));
  addch('[');
  for(unsigned int i = 0; i<s.size(); ++i) {
    if (needs_highlight && s[i] == c) {
      attrset(A_BOLD | COLOR_PAIR(1));
      addch(c);
      attrset(A_NORMAL | COLOR_PAIR(1));
      needs_highlight = false;
    }
    else
      addch(s[i]);
  }
  addch(']');
}

void output(state &s, Screen &screen, std::vector<std::string> &logbufdata) {
 
  int rows = screen.get_rows();
  int cols = screen.get_cols();


  // draw the labels
  for(int r = 0; r < rows; ++r) {
    move(Y(r,-1), X(r,-1));
    printw("%i", r%10);
    
    if (r/10 > 0) {
      move(Y(r,-1), X(r,-1)-1);
      printw("%i", r/10);
    }
  }

  for(int c = 0; c < cols; ++c) {
    move(Y(-1,c), X(-1,c));
    printw("%i", c%10);
    
    if (c/10 > 0) {
      move(Y(-1,c)-1, X(-1,c));
      printw("%i", c/10);
    }
  }

  // draw the map
  for(int r = 0; r < rows; ++r) {
    for(int c = 0; c < cols; ++c) {
      move(Y(r,c), X(r,c));
      
      switch(screen.read_color(r, c)) {
        case GRAY: 
          attrset(GRAY_COLOR);
          break;
        case BLUE: 
          attrset(WATER_COLOR);
          break;
        case GREEN: 
          attrset(A_BOLD | COLOR_PAIR(4));
          break;
        case RED: 
          attrset(A_BOLD | COLOR_PAIR(3));
          break;
        default:
          attrset(GRAY_COLOR);
          break;
      }

      addch(screen.read(r, c));
      addch(' ');
    }
  }

  // Draw the interface

  int yy = Y(rows + 1, 0);
  int xx = 2; //X(rows + 1, 0);

  attrset(A_NORMAL | COLOR_PAIR(1));
  move(yy, xx);
  printw("Ships: %i / %i  ", s.ships, s.initial_ships);
  move(yy, xx + 20);
  printw("Round: %i ", s.round);
  
  if (!s.alive) {
    move(yy, xx + 36);
    attrset(A_BOLD | COLOR_PAIR(3));
    printw("GAME OVER");
    attrset(A_NORMAL | COLOR_PAIR(1));
  }
  if (s.alive && s.ships == 0) {
    move(yy, xx + 36);
    printw("YOU WON!");
  }

  move(yy+2, xx);
  print_hint("Quit", 'Q');
  
  move(yy+2, xx+10);
  if (s.play)
    print_hint("Pause", 'P');
  else {
    print_hint("Play", 'P');
    addch(' ');
  }

  move(yy+2, xx+20);
  print_hint("Step", 'S') ;
  
  move(yy+2, xx+30);
  print_hint("Fast-forward", 'F') ;
 
  /* Log */
  attrset(GRAY_COLOR);
  move(yy+4, xx);
  for (unsigned int i = 0; i < log_line_length; i++) {
    addch('-');
  }
  move(yy+4+9, xx);
  for (unsigned int i = 0; i < log_line_length; i++) {
    addch('-');
  }
  move(yy+4, xx + log_line_length - 20);
  addch('|');
  printw(" Captain's log ");
  addch('|');

  for (unsigned int i = 0; i < logbufdata.size(); ++i) {
    move(yy+5 + i, xx);
    clrtoeol();
  }

  attrset(A_NORMAL | COLOR_PAIR(1));
  for (unsigned int i = 0; i < logbufdata.size(); ++i) {
    move(yy+5 + i, xx);
    printw("%s", logbufdata[i].c_str());
  }

  refresh();
}
