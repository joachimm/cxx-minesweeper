A c++ minesweeper library.

Contains an example cli gui client.
A library containing the minesweeper engine.
A minesweeper solver, uses the Eigen 3 Matrix Library to solve the gameboard.

build the solver.
clang -std=c++11 -O -I/opt/local/include/eigen3 -I/opt/local/include/ mine-solver.cc minesweeper.cc draw.cc

build the game.
clang -std=c++11 -O -I/opt/local/include/eigen3 -I/opt/local/include/ play.cc minesweeper.cc draw.cc