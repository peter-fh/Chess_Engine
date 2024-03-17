rm IllegalMoves.txt
echo "## Compiling ##"
g++ -o driver Driver.cpp Board.cpp Bithack.cpp Move.cpp Moves.cpp
echo "## Compiled, Running ##"
./driver
echo "## Complete ##"
rm driver

