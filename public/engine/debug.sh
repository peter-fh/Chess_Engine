#rm IllegalMoves.txt
echo "## Compiling ##"
g++ -o debug Debug.cpp Engine.cpp Board.cpp Bithack.cpp Move.cpp Moves.cpp
echo "## Compiled, Running ##"
./debug
echo "## Complete ##"
rm debug 
