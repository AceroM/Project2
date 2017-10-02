
#ifndef GUN_H
#define GUN_H

#include "common.h"

class Gun {
  result (*oracle) (int row, int col);
public : 
  Gun(result (*oracle) (int, int));
  result shoot(int row, int col);
};

#endif
