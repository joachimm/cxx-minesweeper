#include "draw.h"
void draw_board(minesweeper_t const& game, bool revealMines, std::ostream& stream)
{
	stream << std::string(game.getLengthX() + 2, '-') << std::endl;
	for(int y = 0; y < game.getLengthY(); ++y)
	{
		stream << "|";
		for(int x = 0; x < game.getLengthX(); ++x)
		{
			auto state = game.get_mined_state(x, y);
			switch(state)
			{
				case minesweeper_t::none:
				stream << (game.is_revealed(x, y) ? "." : " ");
				break;
				case minesweeper_t::mined:
				stream << (revealMines ? "X" : " ");
				break;
				default:
				stream << (game.is_revealed(x, y) || revealMines ? std::to_string(static_cast<char>(state)) : " ");
				break;
			}
		}
		stream << "|" << std::endl;
	}
	stream << std::string(game.getLengthX() + 2, '-') << std::endl;

}
