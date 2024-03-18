#include <stdbool.h>
#include "Move.h"
using namespace std;


Move::Move(): type(-1) {}


Move::Move(int from, int to, int inptype, int inptake) {
    squares[FROM] = from; 
    squares[TO]= to;
    type = inptype;
    take = inptake;    
}


string Move::moveCode(){

    if (isNull()){
        return "null";
    }

    char pieces[6] = {'K', 'Q', 'R', 'B', 'N', 'P'};
    char columns[8] = {'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};
    char column = columns[squares[TO] % 8];
    string row = to_string(squares[TO]/8 + 1);
    char piece = pieces[type];

    string take_num = to_string(take);
    if (take >= 0){
        return string() + piece + 'x' + column + row + " Take: " + take_num;
    }


    return string() + piece + column + row + " Take: " + take_num;
}


bool Move::isNull(){
    if (type == -1)
        return true;
    return false;
}

bool Move::equals(Move move){
    if (!(move.squares[0] == squares[0] && move.squares[1] == squares[1]))
        return false;

    if (!(move.type == type))
        return false;

    if (!(move.take == take))
        return false;

    return true;
}




