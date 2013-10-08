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

int test_cout = 0;
struct state{
	string puzzle;
	int G;
	int H;
	state *parent;

};

struct myComparator{
	bool operator()(state* lhs, state* rhs){
		return (( lhs->G + lhs->H) > (rhs->G + rhs->H));
	}
};

int computingH1(string puzzle){
	int h1 = 0;
	int i = 0;
	for (int i = 0; i < puzzle.length(); i++){
		if (((int)(puzzle[i] - '0') != i) && ((int)(puzzle[i] - '0') != 0))
			h1 ++;
	}
	return h1;
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

void astar(string puzzle) {
	priority_queue <state*, vector<state*>,myComparator> frontier;
	map <string, state*> explored_sets;
	map <string, state*> open_list;			//store all nodes put on open_list(frontier)
	state *root_state = new state;
	
	root_state->puzzle = puzzle;
	root_state->parent = NULL;
	root_state->G = 0;
	root_state->H = computingH2(root_state->puzzle);
	root_heuristic = root_state->H;
		
	frontier.push(root_state); 						//push into the frontier queue
	explored_sets.insert(pair<string, state*> (puzzle, root_state));	//marked as explored
	open_list.insert(pair<string, state*> (puzzle, root_state));
	
	string next_state;
	while (!frontier.empty()){			//do until no frontier
		state *front = frontier.top();		//explore the first frontier
		explored_sets.insert(pair<string, state*> (front->puzzle, front));	//marked as explored
		frontier.pop();
		number_goals_tested ++;			//whenver examine isGoal, increment
		if (isGoalState(front->puzzle)){

				solution_depth = front->G;	//depth would be the depth of its parent+1
				//find the target, return the depth, queue size etc..
				return;
			}						//find the solution
		for (int direction = UP; direction <= LEFT; direction ++){
			next_state = nextState(front->puzzle,direction);
			if (next_state == "") {
				continue;	//if cannot access, continue iterating
			}
			if (explored_sets.find(next_state) != explored_sets.end()) {	//already on explored list
				continue;	//ignore
			}
			
			if (open_list.find(next_state) != open_list.end()) {		//already on open_list, check G
				int new_G = front->G + 1;
				if (new_G < (open_list.find(next_state)->second->G)){			//the new path is better
					open_list.find(next_state)->second->parent = explored_sets.find(front->puzzle)->second;		//change the parent in the open_list map
					open_list.find(next_state)->second->G = new_G;
				}
				// if not smaller G, do nothing.
			}

			else if (open_list.find(next_state) == open_list.end()){ //has not been pushed to frontier yet


				state *new_state = new state;
				new_state->puzzle = next_state;							//store the depth and puzzle
				new_state->G = front->G + 1;
				new_state->H = computingH2(next_state);
				new_state->parent = explored_sets.find(front->puzzle)->second;//point the parent to the closed list node
				open_list.insert(pair<string, state*> (new_state->puzzle,new_state));	//insert the node to frontier map
				frontier.push(new_state);										//push to frontier
				frontier_max_size  = (frontier_max_size < frontier.size()) ? frontier.size():frontier_max_size;														//update the max_frontier_size
			
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
	int sum_heuristic[10] = {0};
	int C_G;								//indicator
	double avg_solution_depth[10], avg_goals_tested[10], avg_max_frontier_size[10], avg_heuristic[10];

    while(next_puzzle.size()){
	    puzzle_counter++;
        astar(next_puzzle);
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
        
        solution_depth = 0;
        number_goals_tested = 0;
        frontier_max_size = 1;
        root_heuristic = 0;
        next_puzzle = puz.nextPuzzle();
    }

    cout << "       C_G   |  Heuristic   |   Avg depth      |    Avg # goals   |   Avg max size\n";
    for (int i = 0; i <10; i ++){
    	avg_solution_depth[i] = (double)sum_solution_depth[i]/(double)number_in_CG[i];
    	avg_goals_tested[i] = (double)sum_goals_tested[i]/(double)number_in_CG[i];
    	avg_max_frontier_size[i] = (double)sum_max_frontier_size[i]/(double)number_in_CG[i];
    	avg_heuristic[i] = (double)sum_heuristic[i]/(double)number_in_CG[i];
		cout << setw(10) << i+1  <<"   |" << setw(11) << avg_heuristic[i]<< "   |" << setw(15) << avg_solution_depth[i] <<"   |" << setw(15) << avg_goals_tested[i] << "   |" << setw(15) << avg_max_frontier_size[i] <<endl;
		}
}
