#include <stdbool.h>
#include <iostream>
#include "board.h"
using namespace std;


string Move::moveCode(){

    if (isNull()){
        return "null";
    }

    char pieces[6] = {'K', 'Q', 'R', 'B', 'N', 'P'};
    char columns[8] = {'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};
    char column = columns[squares[TO] % 8];
    string row = to_string(squares[TO]/8 + 1);
    char piece = pieces[type];

    if (take){
        return string() + piece + 'x' + column + row;
    }

    return string() + piece + column + row;
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


int main(){
    Move move;
    cout << move.moveCode() << "\n";
    move.squares[0] = 55;
    move.squares[1] = 63;
    move.type = 1;
    move.take = true;
    cout << move.moveCode() << "\n";


}


