#include "minesweeper.h"
#include "draw.h"

void play(minesweeper_t& game)
{
	int x = 0;
	int y = 0;

	do
	{
		draw_board(game, false, std::cout);
		std::cout << "x[0-" << game.getLengthX() - 1 <<"] y:[0-" << game.getLengthY() - 1 <<"]" << std::endl;
		std::cin >> x;
		while(x < 0 || x >= game.getLengthX())
		{
			std::cout << "x must be bigger than zero and smaller than:" << game.getLengthX() << std::endl;
			std::cin >> x;
		}
		std::cin >> y;
		while(y < 0 || y >= game.getLengthY())
		{
			std::cout << "y must be bigger than zero and smaller than:" << game.getLengthY() << std::endl;
		}
	}
	while(game.play(x, y));

	if(game.is_won())
	{
		std::cout<< "Congratulations you won" << std::endl;
	}
	else
	{
		std::cout<< "Ouch!! Better luck next time" << std::endl;
	}

	draw_board(game, true, std::cout);
	
	
}

minesweeper_t startGame()
{
	int x = 0;
	int y = 0;

	while(x < 1)
	{
		std::cout << "Squares per horizontal edge" << std::endl;
		std::cin >> x;
		if(x < 1)
		{
			std::cout << "Number must be bigger than zero" << std::endl;
		}
	}

	while(y < 1)
	{
		std::cout << "Squares per vertical edge" << std::endl;
		std::cin >> y;
		if(y < 1)
		{
			std::cout << "Number must be bigger than zero" << std::endl;
		}
	}
	int mines = 0;

	while(mines < 1 || mines > x * y)
	{
		std::cout << "Number of mines" << std::endl;
		std::cin >> mines;
		if(mines > x * y)
		{
			std::cout << "Too many mines!" << std::endl;
		}
	}

	return minesweeper_t(x, y, mines);
}

int main()
{
	minesweeper_t game = startGame();
	while(true)
	{
		play(game);
		std::cout << "Play again (1)" << std::endl;
		std::cout << "Resize board (2)" << std::endl;
		std::cout << "Quit (3)" << std::endl;
		int choice;
		std::cin >> choice;
		if(choice == 1)
		{
			game.setup();
		}
		else if(choice ==2)
		{
			game = startGame();
		}
		else
		{
			// exit
			return 0;
		}
	}
}
