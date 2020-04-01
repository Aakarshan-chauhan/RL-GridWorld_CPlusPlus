#include<iostream>
#include<map>
#include<vector>
#include<set>

using namespace std;

// Class to interact and form our gameboard
class Grid {
	public:
		
		//i and j are coordinates of the agent in the grid, y axis is i and x axis is j. Top left corner is (0, 0).
		int rows, cols, i, j;

		//rewards is a ditionary with set of coordinates mapped with numerical rewards for that state.
		std::map<std::vector<int>, float> rewards;
		
		//actions is a dictionary with set of coordinates mapped to the list of possible actions in that state.
		std::map<std::vector<int>, std::vector<char>> actions;
		
		Grid(int num_rows, int num_cols, std::vector<int> start_coord)
		{
			rows = num_rows;
			cols = num_cols;
			i = start_coord[0];
			j = start_coord[1];
		}

		void set(std::map<std::vector<int>, float> reward, std::map<std::vector<int>, std::vector<char>> action)
		{
			rewards = reward;
			actions = action;
		}
		
		// Set the state of the agent by assigning the coordinates to the agent.
		void set_state(std::vector<int> s)
		{
			i = s[0];
			j = s[1];
		}

		// Return the immidiate coordinates at the time of call.
		std::vector<int> current_state()
		{
			return { i, j };
		}

		// Return a boolean if agent is in terminal state or not.
		bool is_terminal(std::vector<int> s)
		{
			for (const auto x : actions)
				if (s == x.first)
					return false;
			return true;
		}

		// Move the agent by changing its coordinate if the assigned action is a possible action in that state.
		int move(char action)
		{
			for (auto x : actions[{i, j}])
			{
				if (action == x)
				{
					if (action == 'U')
						i--;
					else if (action == 'D')
						i++;
					else if (action == 'L')
						j--;
					else if (action == 'R')
						j++;
					break;
				}
			}
			return rewards[{i, j}];
		}

		void undo_move(char action)
		{
			if (action == ' U')
				i++;
			else if (action == 'D')
				i--;
			else if (action == 'L')
				j++;
			else if (action == 'R')
				j--;
		}

		//Return False if the agent has reached a terminal state or an invalid state.
		bool game_over()
		{
			std::vector<int> temp = { i, j };
			for (const auto x : actions)
				if (temp == x.first)
					return false;
			return true;
		}

		// Return all possible states.
		std::vector<std::vector<int>> all_states()
		{
			std::vector<std::vector<int>> all_states;
			std::set<std::vector<int>> set_states;
			for (const auto x : actions)
				set_states.insert(x.first);
			for (const auto y : rewards)
				set_states.insert(y.first);
			for (const auto z : set_states)
				all_states.push_back(z);
			return all_states;
		}
};


Grid standard_grid()
{
	/* 

	Define a grid that shows the board and reward of 
	each file with possible actions at each state.
	Our current board is a 3 x 4 board. Example:
	.  .  .  W
	.  X  .  L
	S  .  .  .
	W = Win, X = wall, L = loss, S = start position.

	*/
	Grid g = { 3, 4, {2, 0} };
	std::map<std::vector<int>, float> rewards;
	rewards[{0, 3}] = 1;
	rewards[{1, 3}] = -1;
	std::map<std::vector<int>, std::vector<char>> actions;
	std::vector<std::vector<int>> blocks = { {1,1}, {0,3}, {1,3} };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{	
			std::vector<int> pos = { i, j };
			std::vector<char> list;
			if (std::find(blocks.begin(), blocks.end(), pos) == blocks.end())
			{
				if (i + 1 != 1 && i + 1 < 3)
					list.push_back('D');
				if (i - 1 != 1 && i - 1 > 0)
					list.push_back('U');
				if (j + 1 != 1 && j + 1 < 4)
					list.push_back('R');
				if (j - 1 != 1 && j - 1 > 0)
					list.push_back('L');
			}
			actions[{i, j}]= list;
		}
	}
	g.set(rewards, actions);
	return g;
}

// This will be a grid with each move or action resulting in a small penalty.
// This would encourage shorter paths over longer ones.
Grid negative_grid(float step_cost = -0.1)
{
	Grid g = standard_grid();
	for (const auto x : g.rewards)
	{
		g.rewards[x.first] = step_cost;
	}
	return g;
}