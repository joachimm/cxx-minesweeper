#include "minesweeper.h"
std::vector<minesweeper_t::position_t> const minesweeper_t::adjacent = {
		{0,0}, {1,0}, {2,0},
		{0,1}, /*X*/  {2,1},
		{0,2}, {1,2}, {2,2}
	};
