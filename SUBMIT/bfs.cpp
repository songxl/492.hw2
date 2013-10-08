#include <iostream>
#include <queue>
#include <map>
#include <iomanip>
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

struct state{
	string puzzle;
	int depth;
};

void bfs(string puzzle) {
	queue <state> frontier;
	map <string,int> explored_sets;
	state root_state;
	state new_state;
	root_state.puzzle = puzzle;
	root_state.depth = 0;
	number_goals_tested ++;			//test the root puzzle first
	if (isGoalState(root_state.puzzle)){	//if the given puzzle is the solution?
		solution_depth = root_state.depth;
		frontier_max_size = 0;
		return;
	}
	
	frontier.push(root_state); 			//push into the frontier queue
	explored_sets.insert(pair<string, int> (puzzle, 0));	//marked as explored

	string next_state;
	while (!frontier.empty()){			//do until no frontier
	state front = frontier.front();		//pop the frontier
	frontier.pop();
		for (int direction = UP; direction <= LEFT; direction ++){
			next_state = nextState(front.puzzle,direction);
			if (next_state == "") continue;	//if cannot access, continue iterating
			number_goals_tested ++;			//whenver examine isGoal, increment
			if (isGoalState(next_state)){
				solution_depth = front.depth + 1;	//depth would be the depth of its parent+1
				return;
			}						//find the solution
			else if (explored_sets.find(next_state) == explored_sets.end()){ //has not visited yet
				new_state.puzzle = next_state;							//store the depth and puzzle
				new_state.depth = front.depth + 1;
				frontier.push(new_state);								//push to frontier
				explored_sets.insert(pair<string, int> (new_state.puzzle,new_state.depth));	//insert the visited node and its depth									 
				frontier_max_size  = (frontier_max_size < frontier.size()) ? frontier.size():frontier_max_size;												//update the max_frontier_size
			
			}
		}

	}	
}

int main() {
    Puzzles puz("puzzles.txt");
    string next_puzzle = puz.nextPuzzle();
    
	int sum_solution_depth[10] = {0};		
	int sum_goals_tested[10] = {0};
	int sum_max_frontier_size[10] = {0};
	int number_in_CG[10] = {0};				//count how many puzzles in a configuration group
	int C_G;								//indicator
	double avg_solution_depth[10], avg_goals_tested[10], avg_max_frontier_size[10];

    while(next_puzzle.size()) {
	    puzzle_counter++;
        bfs(next_puzzle);
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
        solution_depth = 0;
        number_goals_tested = 0;
        frontier_max_size = 1;
        next_puzzle = puz.nextPuzzle();
    }

    cout << "  C_G   |   Avg depth   |    Avg # goals   |   Avg max size\n";
    for (int i = 0; i <10; i ++){
    	avg_solution_depth[i] = (double)sum_solution_depth[i]/(double)number_in_CG[i];
    	avg_goals_tested[i] = (double)sum_goals_tested[i]/(double)number_in_CG[i];
    	avg_max_frontier_size[i] = (double)sum_max_frontier_size[i]/(double)number_in_CG[i];
		cout << setw(5) << i+1 << "   |" << setw(12) << avg_solution_depth[i] <<"   |" << setw(15) << avg_goals_tested[i] << "   |" << setw(15) << avg_max_frontier_size[i] <<endl;
	}
}
