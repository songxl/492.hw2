#include <iostream>
#include <fstream>

using namespace std;

/* 
   Description : 
   class for iterating over the puzzles
   Usage : Puzzle puz("File Name of the file relative to current directory")
           puz.nextPuzzle() : gives the next puzzle in the form of a string.
           For example : 0 1 2
                         3 4 5
                         6 7 8 
                         would be returned as "012345678"
           
           readFullLine() is of no use to students
*/

class Puzzles
{
      public:
      ifstream ifile;
      Puzzles (string file) {
        ifile.open (file.c_str (), ios::in);
      }

      void readFullLine ();
      string nextPuzzle ();
};

// Not usable to students
void swap (string current, int old_zero, int new_zero);

/*
    Function that returns the next state where 
    string current is the current state of the puzzle in the string format
    unsigned move is which direction you want the zero in the current state to move
    move = 1 would move the zero UP 
        Example : 123045678 -> nextState(UP=1) -> 023145678
    move = 2 would move the zero DOWN 
        Example : 012345678 -> nextState(DOWN=2) -> 321045678
    Similarly move = 3 would move the zero to RIGHT and move = 4 would the zero to LEFT
    if a certain move cannot be performed like move  = 1 on 012345678 would not do anything, NULL is returned
*/
string  nextState (string current, unsigned move);

/* returns a true if the state="012345678" and false otherwise */
bool isGoalState(string state);
