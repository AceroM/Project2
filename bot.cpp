/*
Author: Miguel Acero
Instructor: Mike Zamansky
CSCI 135 - Program 2 - Game Battleship
This program attempts
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "bot.h"
#include "screen.h"
#include <vector>
 
using namespace std;
/*
To do list
1) store possible choices in arrays for position function
2) store length of every possible ships and try to finish those firsts?
3) create better surrounding 1s for sunk ship
4) Add more specific guessing?
*/
 
int ROWS;
int COLS;

int iter = 0; // records iteraton
int currentR; //variable to store r in after every next turn
int currentC; //variable to store c in after every next turn
const int MAXBOARDSIZE = 35;
int board[MAXBOARDSIZE][MAXBOARDSIZE];
bool is_Miss = false; //for recording miss
bool found = false; //for recording a hit 
int direction = 2  ; 
int hit = 0; //we need this to record how many hits on each hunt
int secondH = 0; //another variable to record how many hits
int lastDirection = 0; //to store the last direction

 
/* Initialization procedure, called when the game starts:
 
   init (rows, cols, num, screen, log)
 
   Arguments:
    rows, cols = the boards size
    num        = the number of ships
    screen     = a screen to update your knowledge about the game
    log        = a cout-like output stream
*/

void init(int rows, int cols, int num, Screen &screen, ostream &log)
{
  ROWS = rows;
  COLS = cols;
  log << "Start." << endl;

  for (int i = 0; i < ROWS; i++) { 
    for (int j = 0; j < COLS; j++){
      board[i][j] = 0; //fills the boards with 0s
    }
  }

  for (int i = 0; i > ROWS-1; i++) {
    for (int j = 0; j > COLS-1; j++){
      board[i][j] = 4; //fills the out of bounds with 4s
    }
  }
}
 
 
/* The procedure handling each turn of the game:
 
   next_turn(sml, lrg, num, gun, screen, log)
 
   Arguments:
    sml, lrg = the sizes of the smallest and the largest ships that are currently alive
    num      = the number of ships that are currently alive
    gun      = a gun.
               Call gun.shoot(row, col) to shoot:
                  Can be shot only once per turn.
                  Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
    screen   = a screen to update your knowledge about the game
    log      = a cout-like output stream
*/

/*
rule of thumb
0 = empty 
1 = hit
2 = hit n sunk
3 = miss
4 = out of bounds

directions
0 = goes down
1 = goes up
2 = goes left
3 = goes right
*/

//ended up never using this
// void smartGuessing(int board[][35], int r, int c){

//     vector<int> x;
//     vector<int> y;
//     for (int i = 0; i < ROWS; i++) {
//         for (int j = 0; j < COLS; j++){
//             if (board[r-1][c] == 0 && board[r+1][c] == 0 && board[r][c+1] == 0 && board[r][c-1] == 0){
//                x.push_back(r);
//                y.push_back(c);
//             }
//         }
//     }

//     int RandIndex = rand()%x.size();
//     r = x[RandIndex];
//     c = y[RandIndex];
// }

void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log){
    int r = 0;
    int c = 0;
    
    while(board[r][c] > 0){ //first check to make sure you are on an empty square
        r = rand() % ROWS;
        c = rand() % COLS;
    }    

    if (found == true){ //if first turn = hit, begins hunting mode
        hit += 1; //neccesary for storing how many hits
        secondH += 1;

        if(secondH == 0){
             direction = rand()%4; // This basically randomizes the second hit after the first hit is found (around the first hit)
        }

        if(is_Miss){ //if second turn is a miss
            direction += 1; //changes the direction to 1
            hit = 1; //changes to 1 so that it resets back to first instance of the hit and move on from there, since the x marks a dead
            // }
        }

        if(direction == 4){ //when the direction goes to 4, we need to reset it
            direction = 0;
        }

        if (direction == 0){ 

            r = currentR + hit; // goes south 
            c = currentC; 

            if(board[r][c] > 0 || r < 0 || r > ROWS-1){ //if the next hit is not empty or if the next hit is greater than ROWS-1 (-1 compensates for board size)
                direction=1; //changes the direction
                hit=1;
            }
        }      

        if (direction == 1){  
            r = currentR - hit; //goes north
            c = currentC;

            if (board[r][c] > 0 || r < 0){ //if board is not empty on the next hit or if the position is out of bounds on the top
                direction = 2; //change it up and go left
                hit = 1;
            }
        }

        if (direction == 2){ //if direction == west
            r = currentR;
            c = currentC-hit;

            if(board[r][c] > 0 || c < 0){ //if value is not empty or if the position is out of bounds on the left side
                direction = 3; //change direction to go right
                hit = 1; //hit set back to 1
            }
        }

        if (direction == 3){ 
            r = currentR;
            c = currentC + hit; //goes east 
            if(board[r][c] > 0 || c < 0 || board[r][c] > COLS-1){ //if value
                direction = 0;
                hit = 1;
            }  
        }
        lastDirection = direction;
    }

    //basically I'm just trying to make it really spread out
    else if (found == false){ //if misses
        if (iter == 0) { //start at center for some reason
            r = ROWS/2;
            c = COLS/2;
        }

        else if (iter%2 == 0){ // on some iterations tries to guess in the center
            r = rand() % ROWS;
            c = rand() % COLS;
            while(r < ROWS/4) r = rand()%ROWS; 
            while(c < COLS * (3/4)) c = rand()%COLS;  
        }

        else if (iter%3 ==0){
            r = rand() % ROWS/2;
            c = rand() % COLS/2;
        }
        //just finding neighboring ships.
        else if(lastDirection == 0){ 
            r = currentR - 1;
            c = currentC + 2;
            if(board[r][c] > 0){ 
                c = currentC - 2;
                r = currentR + 1;
            }
        }

        else if(lastDirection == 1){
            r = currentR + 1;
            c = currentC - 2;
            if(board[r][c] > 0){
                c = currentC + 2;
                r = currentR - 1;
            }
        }

        else if(lastDirection == 2){ 
            r = currentR - 2;
            c = currentC + 1;
            if (board[r][c] > 0){
                r = currentR + 2;
                c = currentC - 1;
            }
        }

        else if(lastDirection == 3){
            r = currentR - 2;
            c = currentC - 1;
            if (board[r][c] > 0){
                r = currentR + 2;
                c = currentC + 1;
            }
        }
    }

    while(board[r][c] != 0 || c < 0 || r < 0){ //second check to make sure theyre inbounds.
        r = rand()%ROWS;
        c = rand()%COLS;
    }

    result res=gun.shoot(r, c);        

    if (res == HIT){
        screen.mark(r, c, 'O', GREEN);
        board[r][c] = 1;
        if(found == false){
            currentR = r;
            currentC = c;
        }

        is_Miss = false;
        found = true;
    }
   
    else
    if (res == HIT_N_SUNK){ //if sunk
        screen.mark(r, c, 'S', RED);
        secondH = 0; //resets to 0 so that the next second hit will be randomized again.
        board[r][c] = 2; //marks all hit n sunk with 2's
        if(direction == 0){ //if north
            int nextPos = r-1; //need this variable to store the next position
            while(board[nextPos][c] == 1){ //for every next position
                board[nextPos][c+1] = 3;
                screen.mark(nextPos, c+1, 'x', BLUE);
                board[nextPos][c-1] = 3;
                screen.mark(nextPos, c-1, 'x', BLUE);
                nextPos--;           
            }
        board[nextPos][c]=3;
        screen.mark(nextPos, c, 'x', BLUE);
        }
 
        if(direction == 1){ //if south
            int nextPos = r+1;
            while(board[nextPos][c] == 1){
                board[nextPos][c+1] = 3;
                screen.mark(nextPos, c+1, 'x', BLUE);
                board[nextPos][c-1] = 3;
                screen.mark(nextPos, c-1, 'x', BLUE);
                nextPos++;
            }

            nextPos = r-1;
            while(board[nextPos][c] == 1){
                board[nextPos][c+1] = 3;
                screen.mark(nextPos, c+1, 'x', BLUE);
                board[nextPos][c-1] = 3;
                screen.mark(nextPos, c-1, 'x', BLUE);
                nextPos--;   
            }
        }
 
        if(direction == 2){
            int nextPos = c+1; //if east
            while(board[r][nextPos] == 1){
                board[r+1][nextPos] = 3;
                screen.mark(r+1, nextPos, 'x', BLUE);
                board[r-1][nextPos] = 3;
                screen.mark(r-1, nextPos, 'x', BLUE);
                nextPos++;
            }
               
            board[r][nextPos] = 3;
            screen.mark(r, nextPos, 'x', BLUE);
        }

        if(direction == 3){
            int nextPos = c-1; //if west
            while(board[r][nextPos] == 1){
            board[r+1][nextPos] = 3;
            screen.mark(r+1, nextPos, 'x', BLUE);
            board[r-1][nextPos] = 3;
            screen.mark(r-1, nextPos, 'x', BLUE);
            nextPos--;
            }
           
            nextPos = c+1;
            while(board[r][nextPos] == 1){
                board[r+1][nextPos] = 3;
                screen.mark(r+1, nextPos, 'x', BLUE);
                board[r-1][nextPos] = 3;
                screen.mark(r-1, nextPos, 'x', BLUE);
                nextPos++;
            }
        }
 
        //after marking the things that connect to S as sunk we need to mark X's around sunk that ARE NOT green. 
        if(board[r-1][c] != 1){
            board[r-1][c] = 3;
            screen.mark(r-1, c, 'x', BLUE);
        }

        if(board[r][c+1] != 1){
            board[r][c+1] = 3;
            screen.mark(r, c+1, 'x', BLUE);
        }

        if(board[r][c-1] != 1){
            board[r][c-1] = 3;
            screen.mark(r, c-1, 'x', BLUE);
        }

        if(board[r+1][c] != 1){
            board[r+1][c] = 3;
            screen.mark(r+1, c, 'x', BLUE);
        }

        is_Miss = false;
        found = false;

        hit = 0;
        direction=0;
    }

    else
    if (res == MISS){
        screen.mark(r, c, 'x', BLUE);
        board[r][c] = 3; //marks every element in the board as 3s 
        is_Miss=true;
    }
    //logged neccesary information for debugging
    log << "r: " << r << " c: " << c << " d: " << direction << " board[r][c] = " << board[r][c] << " iter: " << iter << endl;
    log << " hit: " << hit << " is_Miss: " << is_Miss << " found: " << found << " secondH: " << secondH << endl;
    iter += 1;
}
