#include <iostream>
#include <iomanip>
#include <queue>
#include <map>

#include "parse.h"

using namespace std;

const int UP = 1;
const int DOWN = 2;
const int RIGHT = 3;
const int LEFT = 4;

int solution_depth = 0;
int number_goals_tested = 0;
int frontier_max_size = 1;
int puzzle_counter = 0;
int root_heuristic = 0;

int puzzle_solved = 0;
int out_of_choice = 0;

int test_cout = 1;
struct state{
	string puzzle;
	int H;
	int depth;
};

int computingH2(string puzzle){
	int h2 = 0;
	int value;
	for (int i = 0; i < 8; i++){
		value = int(puzzle[i] - '0');
		if (value != 0){
			h2 += abs(i/3 - value/3) + abs( i%3 - value % 3) ;
		}
	}
	return h2;
};

void hillclimbing(string puzzle) {
	state *root_state = new state;
	state *current_state = new state;
	
	root_state->puzzle = puzzle;
	root_state->depth = 0;
	root_state->H = computingH2(root_state->puzzle);
	root_heuristic = root_state->H;
		
	string next_state;
	
	current_state = root_state;
	
	while (!out_of_choice){			//do until no choice or solved the puzzle
		int local_minimum = current_state->H;	//mark as minimum
		string potential_next_state = "";			//mark the potential next state puzzle
		number_goals_tested ++;			//whenver examine isGoal, increment
		
		if (isGoalState(current_state->puzzle)){

				solution_depth = current_state->depth;	//depth would be the depth of its parent+1
				puzzle_solved = 1;
				return;
			}						//find the solution

		for (int direction = UP; direction <= LEFT; direction ++){
			next_state = nextState(current_state->puzzle,direction);
			if (next_state == "") {
				continue;	//if cannot access, continue iterating
			}
			if (computingH2(next_state) < local_minimum){			//find a local minimum
				potential_next_state = next_state;
				local_minimum = computingH2(next_state);
				continue;
			}
		}				
			if (potential_next_state != "") {						//find a local minimum
				current_state->puzzle = potential_next_state;
				current_state->H = computingH2(potential_next_state);
				current_state->depth ++;
			}
			else
				out_of_choice = 1;
		
	}
		
}

int main() {
    Puzzles puz("puzzles.txt");
    string next_puzzle = puz.nextPuzzle();
    
	int sum_solution_depth[10] = {0};		
	int sum_goals_tested[10] = {0};
	int sum_max_frontier_size[10] = {0};
	int number_in_CG[10] = {0};				//count how many puzzles in a configuration group
	int sum_heuristic[10] = {0};
	int sum_puzzle_solved[10] = {0};
	int C_G;								//indicator
	double avg_solution_depth[10], avg_goals_tested[10], avg_max_frontier_size[10], avg_heuristic[10],avg_puzzle_solved[10];

    while(next_puzzle.size()){
	    puzzle_counter++;
        hillclimbing(next_puzzle);
        if ( puzzle_counter <= 2){
        	C_G = 1;
        }
        else if ( puzzle_counter <= 6){
        	C_G = 2;
        }
        else if ( puzzle_counter <= 14){
        	C_G = 3;
        }
        else if ( puzzle_counter <= 30){
        	C_G = 4;
        }
        else if ( puzzle_counter <= 50){
        	C_G = 5;
        }
        else if ( puzzle_counter <= 90){
        	C_G = 6;
        }
        else if ( puzzle_counter <= 156){
        	C_G = 7;
        }
        else if ( puzzle_counter <= 284){
        	C_G = 8;
        }
        else if ( puzzle_counter <= 448){
        	C_G = 9;
        }
        else if ( puzzle_counter <= 764){
        	C_G = 10;
        }
       	number_in_CG[C_G-1] ++;
        sum_solution_depth[C_G - 1] += solution_depth;
        sum_goals_tested[C_G - 1] += number_goals_tested;
        sum_max_frontier_size[C_G - 1] += frontier_max_size;
        sum_heuristic[C_G - 1] += root_heuristic;
        sum_puzzle_solved[C_G - 1] += puzzle_solved;
        
        solution_depth = 0;
        number_goals_tested = 0;
        frontier_max_size = 1;
        root_heuristic = 0;
        puzzle_solved = 0;
        next_puzzle = puz.nextPuzzle();
    }

    //dump your output    	
    cout << "       C_G   |  Heuristic   |   Avg depth      |    Avg # goals   |   Avg max size  | Perct Solved\n";
    for (int i = 0; i <10; i ++){
    	avg_solution_depth[i] = (double)sum_solution_depth[i]/(double)sum_puzzle_solved[i];
    	//calculate the average cost for which you find a solution
    	avg_goals_tested[i] = (double)sum_goals_tested[i]/(double)number_in_CG[i];
    	avg_max_frontier_size[i] = (double)sum_max_frontier_size[i]/(double)number_in_CG[i];
    	avg_heuristic[i] = (double)sum_heuristic[i]/(double)number_in_CG[i];
    	avg_puzzle_solved[i] = (double)sum_puzzle_solved[i]/(double)number_in_CG[i];
		cout << setw(10) << i+1  <<"   |" << setw(11) << avg_heuristic[i]<< "   |" << setw(15) << avg_solution_depth[i] <<"   |" << setw(15) << avg_goals_tested[i] << "   |" << setw(15) << avg_max_frontier_size[i] << "   |" << setw(11) << avg_puzzle_solved[i] <<endl;
		}
}
