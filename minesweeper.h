#ifndef MINESWEEPER_H_F4CBIBGF
#define MINESWEEPER_H_F4CBIBGF

#include <vector>
#include <iostream>
#include <random>
#include <chrono>
// Minesweeper, 
class minesweeper_t {
	struct position_t
	{
		int x;
		int y;
	};
	// offsets for neighboring mines
	static std::vector<position_t> const adjacent;

public:
	enum mined_state {
		none = 0,
		one, two, three, four, five, six, seven, eight,
		mined,
	};

	minesweeper_t (int lengthX, int lengthY, int mines) : 
		lengthX(lengthX),
		lengthY(lengthY),
		mines(mines),
		winCondition(lengthX * lengthY - mines),
		board(lengthX, std::vector<mined_state>(lengthY)),
		revealed(lengthX, std::vector<bool>(lengthY, false))
	{
		setup();
	}

	void setup()
	{
		// initate board, clear revealed
		revealTotal = 0;
		for(int y = 0; y < lengthY; ++y)
		{
			for(int x = 0; x < lengthX; ++x)
			{
				board[x][y] = none;
				revealed[x][y] = false;
			}
		}

		// place mines
	  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distributionX(0, lengthX - 1);
		std::uniform_int_distribution<int> distributionY(0, lengthY - 1);

		int unplaced_mines = mines;
		while(unplaced_mines > 0)
		{
			int x = distributionX(generator);
			int y = distributionY(generator);
			if(!has_mine(x, y))
			{
				set_mined_state(x, y, mined);
				--unplaced_mines;
			}
		}
		
		for(int y = 0; y < lengthY; ++y)
		{
			for(int x = 0; x < lengthX; ++x)
			{
				if(!has_mine(x, y))
				{
					int startX = x - 1;
					int startY = y - 1;
					int neighboring = 0;
					for(auto& offset : adjacent)
					{
						if(has_mine(startX + offset.x, startY + offset.y))
						{
							++neighboring;
						}
					}
					set_mined_state(x, y, static_cast<mined_state>(neighboring));
				}
			}
		}
	}

	mined_state get_mined_state (int x, int y) const
	{
		if(!in_bounds(x, y))
		{
			return none;
		}

		return board[x][y];
	}

	void set_mined_state (int x, int y, mined_state state)
	{
		if(!in_bounds(x, y))
		{
			return;
		}
		board[x][y] = state;
	}

	bool in_bounds (int x, int y) const
	{
		return x >= 0 && x < lengthX && y >= 0 && y < lengthY;
	}

	bool has_mine (int x, int y) const
	{
		return in_bounds(x,y) ? board[x][y] == mined : false;
	}

	bool is_revealed (int x, int y) const
	{
		if(!in_bounds(x, y))
		{
			return false;
		}
		return revealed[x][y];
	}

	//	the game is won when all non mined areas are revealed.
	bool is_won ()
	{
		return revealTotal == winCondition;
	}
	/* updates the reveal table, returns false when game-over*/
	bool play (int x, int y)
	{
		if(!in_bounds(x, y))
		{
			return true;
		}
		revealed[x][y] = true;
		if(has_mine(x, y))
		{
			return false;
		}

		++revealTotal;
		if(is_won())
			return false;
		int startX = x - 1;
		int startY = y - 1;

		for(auto& offset : adjacent)
		{
			if(is_revealed(startX + offset.x, startY + offset.y))
			{
				continue;
			}
			else if(none == get_mined_state(x, y))
			{

				play(startX + offset.x, startY + offset.y);
			}
		}
		return true;
	}

	int getLengthX () const
	{
		return lengthX;
	}
	
	int getLengthY () const
	{
		return lengthY;
	}
private:
	std::vector<std::vector<mined_state> > board;
	std::vector<std::vector<bool> > revealed;
	int revealTotal;
	//	the game is won when all non mined areas are revealed.
	int winCondition;
	int mines;
	int lengthX;
	int lengthY;

};
#endif /* end of include guard: MINESWEEPER_H_F4CBIBGF */
