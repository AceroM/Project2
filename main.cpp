
#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <locale.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "screen.h"
#include "output.h"

int update_from_input(state &s, Screen &screen, std::ostream &gamelog);
void finish(int sig);

/* Run the game */
void run (state &s) {

  buf buf;
  std::ostream gamelog (&buf); // game log
  Screen screen(s.rows, s.cols); // user's screen output

  output(s, screen, buf.data);
  
  { // Bot logic init
    init(s.rows, s.cols, 0, screen, gamelog);
  }

  int k = 0;
  int finished = 0;
  while( !finished ) {
    k++;
    if (k>10) { 
      
      k=0;
      // run the bot's logic and update the state
      if (s.play) {
        update(s, screen, gamelog);
      }

    }
    finished = update_from_input(s, screen, gamelog);
    output(s, screen, buf.data);
    usleep(10000);
  }
}

/* Helper function, puts the value v in the interval [min, max] */
int put_in_range(int v, int min, int max) {
  if (v < min)
    return min;
  else if (v > max)
    return max;
  else
    return v;
}

/* Main function. Mostly boilerplate code of setting up the terminal. */
int main(int argc, char* argv[]){

  /*       1    2    3   4   5   6    */
  /* ARGS: ROWS COLS SML LRG NUM SEED */
  /* Init the game */
  int rows = 20;
  int cols = 20;
  int sml = 1;
  int lrg = 5;
  int num = 50;
  // Random seed
  if (argc > 6) {
    srand(atoi(argv[6]));
  }
  else {
    srand(time(NULL));
    rows += -2 + rand() % 5;
    cols += -2 + rand() % 5;
    num += -5 + rand() % 11;
  }
  if (argc > 1) {rows = put_in_range(atoi(argv[1]), 5, 35);}
  if (argc > 2) {cols = put_in_range(atoi(argv[2]), 5, 35);}
  int max_size = (cols > rows) ? cols : rows; 
  if (argc > 3) {sml = put_in_range(atoi(argv[3]), 1, max_size);}
  if (argc > 4) {lrg = put_in_range(atoi(argv[4]), sml, max_size);}
  if (argc > 5) {num = put_in_range(atoi(argv[5]), 0, rows*cols);}
  // Skip the video simulation?
  bool fast = (argc > 7) && (strcmp(argv[7], "fast") == 0);
 
  // Init the game state
  state st;
  init(st, rows, cols, sml, lrg, num);

  if (!fast) 
  {
    /* User interface initialization */
    signal(SIGINT, finish);

    /* ncurses initialization */
    setlocale(LC_ALL, "");
    initscr();     /* initialize the library and screen */
    cbreak();      /* put terminal into non-blocking input mode */
    nonl();        /* no NL -> CRNL on output */
    noecho();      /* turn off echo */
    start_color();
    curs_set(0);   /* hide the cursor */
    timeout(0);

    use_default_colors();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_CYAN, COLOR_BLACK);

    color_set(0, NULL);
    assume_default_colors(COLOR_WHITE, COLOR_BLACK);

    attrset(A_NORMAL | COLOR_PAIR(0));
    refresh();
    clear();
   
    /* Run the game */
    run(st);
  
    /* Restore the teminal state */
    echo();
    curs_set(1);
    clear();
    endwin();
  }
  else {

    buf buf;
    std::ostream gamelog (&buf); // game log
    Screen screen(st.rows, st.cols); // user's screen output

    // init bot logic
    init(st.rows, st.cols, 0, screen, gamelog);
    
    // run the game until it's over
    while(st.alive && st.ships > 0) { update(st, screen, gamelog); }
    
    printf("%i ", st.round);
    if (st.alive) 
      printf("success\n");
    else
      printf("failure\n");

  }

  return 0;
}

int update_from_input(state &s, Screen &screen, std::ostream &gamelog)
{
    int c;
    int finished=0;

    while ( !finished && (c=getch()) != ERR ) {

      switch(c){
        case 'q': case 'Q':
          finished = 1;
          break;
        case 'f': case 'F':
          for(int i = 0; i<500; ++i){ 
            update(s, screen, gamelog);
          }
          break;
        case 's': case 'S':
          s.play = false;
          update(s, screen, gamelog);
          break;
        case 'p': case 'P':
          s.play = !s.play;
          break;
        default:;
      }

    }                
    return finished;
}

/* SIGINT */
void finish(int sig)
{
  echo();
  curs_set(1);
  clear();
  endwin();
  exit(0);
}
