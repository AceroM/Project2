# Project 2 - Battlefield Board Game
## Note
I submitted it late on blackboard so my tournament results (Karl Hochman) represents my earliest commit with no useful code. My code now is a lot more improved and has a better average round time

This project emulates a single player battlefield game that 
attempts to sink the ship in the shortest possible time

## How I did it
My first thought process was to just scan the board every time and try to make decisions from there but it didn't work out
since the code was getting too complicated and I was getting seg faults. My next idea was to create a vector of 
"Possible choices" and scan through the board everytime and choose from that vector based on different variables. This too
was super complicated, It took me a REALLY LONG TIME to realize I could just use several boolean values as well as a bunch
of variables to store certain values to create what I needed.

0. Init
To start off the entire project I created a 2D array in order to store where ships are either empty, hit, sunk or out of
bounds. starting off with the global variable for max board size which was 35 according to the assignment, I filled
the entire 2d array with 0s which I used to represent empty spaces

 0 = empty
 1 = hit
 2 = hit n sunk
 3 = miss's

```cpp
int ROWS;
int COLS;
const int MAXBOARDSIZE = 35;
int board[MAXBOARDSIZE][MAXBOARDSIZE];

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
```

1. Variables Used
```cpp
	int iter = 0; // records iteraton
	int currentR; //variable to store r in after every next turn
	int currentC; //variable to store c in after every next turn
	const int MAXBOARDSIZE = 35;
	int board[MAXBOARDSIZE][MAXBOARDSIZE];
	bool is_Miss = false; //for recording if the previou
	bool found = false; //for recording a hit 
	int direction = 2  ; 
	int hit = 0; //we need this to record how many hits on each hunt
	int secondH = 0; //another variable to record how many hits
	int lastDirection = 0; //to store the last direction
```

The idea here is that I'm using currentR and currentC as anchor points that record the first hit.
when a shit is hit, bool found == true will begin the hunting mode which will seek to destroy the current ship 
before moving on to the next ship. But how does it do that?
![As you can see here](https://i.gyazo.com/8d700d5a8204b99a033a05e6aca28317.png "Result2")

currentR is being stored in the middle green circle, while the left circle is currentR + hit. This is important
in this code here.

```cpp
		if(is_Miss){ //if second turn is a miss
            direction += 1; //changes the direction to 1
            hit = 1; //changes to 1 so that it resets back to first instance of the hit and move on from there, since the x marks a dead
            // }
        }

        if (direction == 0){ 

            r = currentR + hit; // goes south 
            c = currentC; 

            if(board[r][c] > 0 || r < 0 || r > ROWS-1){ //if the next hit is not empty or if the next hit is greater than ROWS-1 (-1 compensates for board size)
                direction=1; //changes the direction
                hit=1;
            }
        }   
```
> direction = 0 //south
> direction = 1 //north
> direction = 2 //west
> direction = 3 //east

Essentially, if the next hit misses (lets say the top X in the picture), the direction is gonna change +1, in this case 
it'll change to south, but currentR still holds the same value so hit can either go on the top or bottom depending on 
where the first X lies. 

2. Marking X's around sunk ships
for this task I created an important variable nextPos or next position that stored the next value, just like in 
what I used for the previous task. Basically, while the next position is a HIT block, the blocks around it
depending on the direction will be marked with X's/Misses, then nextPos is decremented to find the next hit spot.

```cpp
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
```
the thing about this is it marked X's around all the hit blocks except the Sunk block which I then wrote pretty quickly.
```cpp
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
```

3. Strategy
So I assumed that everyone had a somewhat similar strategy of destroying individual ships one by one, so the the biggest
differentiating code that I can have with someone else is how I look for ships. So if found == false. I don't want my
code to just be

```cpp
	r = rand() % ROWS;
    c = rand() % COLS;
```

This is unreliable. So I googled and researched battlefield strategies and I noticed that a lot of people said that 
the concentration of the ships are mathematically more likely to appear in the middle. since all I needed to start hunting
mode was just one part of the ship, I needed my guesses to be prevalent in the center of the board
![Example map of ship concentrations I found on youtube](https://i.gyazo.com/6ef1cd9189b3f27ca964e6f18c9b3931.png "Concentration" )


```cpp
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

```
I'm pretty sure my code improved my average by a few rounds (Can't say for certain since I submitted late), but running
manual tests seems to prove so.

##Results
![Result 1 - "./battleships	20 20 1 5"](https://i.gyazo.com/e3e37346fa120f749988dce87c9fe094.png "Sample Result")

My results ranged from 80ish-140ish rounds on the default

## Thoughts
This project was difficult really really difficult and I'm so annoyed that It took me such a long time to write code.
but I'm glad with my results since I really learned a lot from this project. I wish I could have thought of solutions
that took more advantage of vectors or dynamic arrays since they were the topics we were learning while this lab was 
happening but I didn't fully absorb those concepts. Anyways the lab was really fun and a great learning experience!
