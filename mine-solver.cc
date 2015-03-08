#include "minesweeper.h"
#include <Eigen/Eigen>
#include <Eigen/Sparse>
#include <Eigen/SVD>
#include <iostream>
#include "draw.h"
#include <math.h>

struct position_t
{
	int x;
	int y;
};

std::vector<position_t> const adjacent = {
	{0,0}, {1,0}, {2,0},
	{0,1}, /*X*/  {2,1},
	{0,2}, {1,2}, {2,2}
};

bool solve()
{
	typedef double numdef;
	typedef Eigen::Matrix<numdef, Eigen::Dynamic, Eigen::Dynamic> matrix_t;
	typedef Eigen::Matrix<numdef, Eigen::Dynamic, 1> Vector;
	size_t lenX = 20;
	size_t lenY = 20;
	size_t numMines = 50;
	size_t numSquares = lenX * lenY;
	size_t numRows = numSquares + 1;
	std::vector<std::vector<bool> > known_mines(lenX, std::vector<bool>(lenY, false));
	minesweeper_t game(lenX, lenY, numMines);
	int guessX = 0;
	int guessY = 0;
	while(game.play(guessX, guessY))
	{
		// Assembly:
		Vector b(numRows);// the right hand side-vector resulting from the constraints

		draw_board(game, false, std::cout);
		matrix_t A(numRows, numSquares);
		A.setZero();

		int ordinal = 0;
		for(int y = 0; y < game.getLengthY(); ++y)
		{
			for(int x = 0; x < game.getLengthX(); ++x)
			{
				// Only look at revealed boards, otherwise we cheat
				if(game.is_revealed(x, y))
				{
					auto state = game.get_mined_state(x, y);
					size_t surrounding_bombs = 0;
					switch(state)
					{
						case minesweeper_t::none:
							surrounding_bombs = 0;
							break;
						case minesweeper_t::mined:
							break;
						default:
							surrounding_bombs = static_cast<char>(state);
						break;
					}
					// Start the walk around this square in upper left square
					int startX = x - 1;
					int startY = y - 1;

					for(auto const pos : adjacent)
					{
						int circleX = startX + pos.x;
						int circleY = startY + pos.y;
						surrounding_bombs -= (game.in_bounds(circleX, circleY) && known_mines[circleX][circleY]) ? 1 : 0;
						if(game.in_bounds(circleX, circleY) && !game.is_revealed(circleX, circleY) && !known_mines[circleX][circleY])
						{
							int xOffset = pos.x - 1;
							int yOffset = lenX * (pos.y - 1); // yes lenX for y value offset
							int actual = (ordinal + xOffset) + yOffset;
							A(ordinal, actual) = 1;
						}
					}
					b(ordinal) = surrounding_bombs;

				}
				++ordinal;
			}
		}

		// Add a final row with all coefficients set to 1
		for(int i = 0; i < numSquares ; ++i)
		{
			A(ordinal, i) = 1;
		}
		b(ordinal) = numMines;

		// Solving:
		Eigen::JacobiSVD<matrix_t> solver;  // performs a SVD factorization of A
		solver.compute(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

		Vector xVec = solver.solve(b);

		int oldGuessX = guessX;
		int oldGuessY = guessY;
		numdef previous = 1;
		// Categorize
		for(int i = 0; i < xVec.size(); ++i)
		{
			numdef value = fabs(xVec(i));
			int x = i % game.getLengthX();
			int y = i / game.getLengthX();
			if(value == 1)
			{
				known_mines[x][y] = true;
			}
			else if(!game.is_revealed(x, y) && !known_mines[x][y] && value < previous)
			{
				previous = value;
				guessX = x;
				guessY = y;
			}
		}
		std::cout << std::endl << "will play x:" << guessX << " y:" << guessY << " with " << (1.0 - fabs(previous) ) * 100.0 << "% weight" << std::endl;
	}
	draw_board(game, true, std::cout);
	return game.is_won();
}

int main(int argc, char** argv)
{
	if(solve())
	{
		std::cout<< "Congratulations you won" << std::endl;
	}
	else
	{
		std::cout<< "Ouch!! Better luck next time" << std::endl;
	}
	return 0;
}