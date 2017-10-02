
#include "screen.h"

bool Screen::in_range (int r, int c) {
  return (r >= 0 && c >= 0 && r < (int)symbols.size() && c < (int)symbols[0].size());
}
  
int Screen::get_rows() {
  return (int)symbols.size();
}

int Screen::get_cols(){
  if (symbols.size() > 0) 
    return (int)symbols[0].size();
  else
    return 0;
}

Screen::Screen(int rows, int cols) {
  if (!(rows > 0 && cols > 0 && rows < 100 && cols < 100)) {
    return;
  }

  symbols.resize(rows);
  for(int r = 0; r < rows; ++r) {
    symbols[r].resize(cols);
    for(int c = 0; c < cols; ++c) {
      symbols[r][c].ch = '-';
      symbols[r][c].clr = BLUE;
    }
  }
}

void Screen::mark(int row, int col, char ch, color clr) {
  if (in_range(row, col)) {
    symbols[row][col].ch = ch;
    symbols[row][col].clr = clr;
  }
}

char Screen::read(int row, int col) {
  if (in_range(row, col)) {
    return symbols[row][col].ch;
  }
  return ' ';
}

color Screen::read_color(int row, int col) {
  if (in_range(row, col)) {
    return symbols[row][col].clr;
  }
  return GRAY;
}

