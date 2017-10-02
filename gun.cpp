
#include "gun.h"

Gun::Gun(result (*oracle)(int, int)) : oracle(oracle) { }

result Gun::shoot(int row, int col) {
  return (*oracle)(row, col);
}
