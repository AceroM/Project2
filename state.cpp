
#include "state.h"

bool in_range(state &s, int r, int c) {
  return (r >= 0 && r < (int)s.map.size() && c >= 0 && c < (int)s.map[0].size());
}

void block(state &s, int r, int c) {
  if (in_range(s, r, c)) {
    s.map[r][c] = BLOCKED;
  }
}

struct loc {
  int r;
  int c;
};

bool place_ship(state &s, int size) {
  std::vector<struct loc> hor; // horizontal
  std::vector<struct loc> ver; // vertical

  // horizontal
  for (int r = 0; r < s.rows; ++r) {
    for (int c = 0; c < s.cols - (size-1); ++c) {
      bool okay = true;
      for(int i = 0; okay && (i < size); ++i) { if(s.map[r][c+i] != EMPTY) okay = false; }
      if (okay) {
        struct loc loc = {r, c};
        hor.push_back(loc);
      }
    }
  }
  // vertical
  for (int c = 0; c < s.cols; ++c) {
    for (int r = 0; r < s.rows - (size-1); ++r) {
      bool okay = true;
      for(int i = 0; okay && (i < size); ++i) { if(s.map[r+i][c] != EMPTY) okay = false; }
      if (okay) {
        struct loc loc = {r, c};
        ver.push_back(loc);
      }
    }
  }

  int num = hor.size() + ver.size();
  if (num > 0) {
    int x = rand() % num;
    if (x < (int)hor.size()) { // if horizontal
      struct loc loc = hor[x];
      for(int i = 0; i < size; ++i) {
        int r = loc.r;
        int c = loc.c + i;
        s.map[r][c] = SHIP; 
        block(s, r-1, c);
        block(s, r+1, c);
      }
      block(s, loc.r, loc.c - 1);
      block(s, loc.r, loc.c + size);
    }
    else { // if vertical
      struct loc loc = ver[x - hor.size()];
      for(int i = 0; i < size; ++i) { 
        int r = loc.r + i;
        int c = loc.c;
        s.map[r][c] = SHIP; 
        block(s, r, c - 1);
        block(s, r, c + 1);
      }
      block(s, loc.r - 1, loc.c);
      block(s, loc.r + size, loc.c);
    }
    return true;
  }
  else 
    return false;

}

void init(state &s, int rows, int cols, int smallest, int largest, int number) {
  s.rows = rows;
  s.cols = cols;
  
  // initialize the map
  s.map.resize(rows);
  for(int r = 0; r < rows; ++r) {
    s.map[r].resize(cols);
    for(int c = 0; c < cols; ++c) {
      s.map[r][c] = EMPTY;
    }
  }
  
  s.round = 1;
  
  s.play = true;
  s.alive = true;

  // place the fleet
  s.ships = 0;
  int can_continue = true;
  while(s.ships < number && can_continue) {
    int size = smallest + rand()%(largest + 1 - smallest);
    bool success = place_ship(s, size);
    if (success)
      s.ships ++;
    else
      can_continue = size > smallest;
  }
  s.initial_ships = s.ships;
 
  // replace BLOCKED by EMPTY
  for(int r = 0; r < rows; ++r) {
    for(int c = 0; c < cols; ++c) {
      if (s.map[r][c] == BLOCKED)
        s.map[r][c] = EMPTY;
    }
  }
}
  

bool ship_alone(state &s, int r, int c) {
  return
    in_range(s,r,c) && (s.map[r][c] != EMPTY)
      && (!in_range(s,r-1,c) || s.map[r-1][c] == EMPTY)
      && (!in_range(s,r+1,c) || s.map[r+1][c] == EMPTY)
      && (!in_range(s,r,c-1) || s.map[r][c-1] == EMPTY)
      && (!in_range(s,r,c+1) || s.map[r][c+1] == EMPTY);
}


// find the largest and the smallest ship sizes
void find_ships(state &s, int &sml, int &lrg) {

  if (s.ships <= 0) {
    sml = 0;
    lrg = 0;
    return;
  }

  lrg = 0;
  sml = s.rows + s.cols;

  // scan horizontally
  for(int r = 0; r < s.rows; ++r) {

    int len = 0;
    bool not_sunk = false;
    
    for(int c = 0; c < s.cols+1; ++c) {
      if (in_range(s,r,c) && s.map[r][c] != EMPTY) {
        len ++;
        if (s.map[r][c] == SHIP)
          not_sunk = true;
      }
      else {
        if (not_sunk && (len > 1 || ship_alone(s,r,c-1))) {
          // ship found
          if (len > lrg) lrg = len;
          if (len < sml) sml = len;
        }
        not_sunk = false;
        len = 0;
      }
    }
  }
  
  // scan vertically
  for(int c = 0; c < s.cols; ++c) {

    int len = 0;
    bool not_sunk = false;
    
    for(int r = 0; r < s.rows+1; ++r) {
      if (in_range(s,r,c) && s.map[r][c] != EMPTY) {
        len ++;
        if (s.map[r][c] == SHIP)
          not_sunk = true;
      }
      else {
        if (not_sunk && (len > 1 || ship_alone(s,r-1,c))) {
          // ship found
          if (len > lrg) lrg = len;
          if (len < sml) sml = len;
        }
        not_sunk = false;
        len = 0;
      }
    }
  }
  
}

namespace { 
  std::vector <std::vector <result> > xrm;
  bool xloaded = false;
  int xrow = -1;
  int xcol = -1;
  
  result xshoot(int row, int col) {

    if (xloaded){
      if (row >= 0 && row < (int)xrm.size() && col >= 0 && col < (int)xrm[0].size()) {
        result res = xrm[row][col];
        xloaded = false;
        xrow = row;
        xcol = col;
        return res;
      }
      else {
        xloaded = false;
        return MISS;
      }
    }

    return ALREADY_SHOT;

  }
  
  bool xalone_alive(state &s, int r, int c) {
    for (int rr = r-1; rr >= 0 && s.map[rr][c] != EMPTY; rr--) { if (s.map[rr][c] == SHIP) return false; }
    for (int rr = r+1; rr < s.rows && s.map[rr][c] != EMPTY; rr++) { if (s.map[rr][c] == SHIP) return false; }
    for (int cc = c-1; cc >= 0 && s.map[r][cc] != EMPTY; cc--) { if (s.map[r][cc] == SHIP) return false; }
    for (int cc = c+1; cc < s.cols && s.map[r][cc] != EMPTY; cc++) { if (s.map[r][cc] == SHIP) return false; }

    return true;
  }

  void xreload (state &s) {
    xloaded = true;
    xrow = -1;
    xcol = -1;
    xrm.resize(s.rows);
    for (int r = 0; r < s.rows; ++r) {
      xrm[r].resize(s.cols);
      for (int c = 0; c < s.cols; ++c) {
        switch (s.map[r][c]) {
          case SHIP:
            if (xalone_alive(s,r,c))
              xrm[r][c] = HIT_N_SUNK;
            else
              xrm[r][c] = HIT;
            break;
          case DMG:
            xrm[r][c] = ALREADY_HIT;
            break;
          default:
            xrm[r][c] = MISS;
            break;
        }
      }
    }
  }
}

void update(state &s, Screen &screen, std::ostream &gamelog) { 
  // don't do anything if the game has finished
  if (!s.alive || s.ships <= 0) return;

  int number = s.ships;
  int smallest = 0;
  int largest = 0;
  find_ships(s, smallest, largest);

  // prepare the gun
  xreload(s);
  Gun gun(xshoot);

  next_turn(smallest, largest, number, gun, screen, gamelog);

  // if shot during the turn
  if (xrow != -1) { 
    switch (s.map[xrow][xcol]) {
      case SHIP:
        if (xalone_alive(s,xrow,xcol)) {
          s.ships--;
        }
        s.map[xrow][xcol] = DMG;
        break;
      default: // otherwise miss
        if (s.ships > 0) { // round increment
          s.round = s.round + 1;
        }
        break;
    }
  }
  else { // if didn't shoot or shot out of bounds
    s.round = s.round + 1;
  }

  if (s.round > s.rows * s.cols) {
    s.alive = false;
  }
}


