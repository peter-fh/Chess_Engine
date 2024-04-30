#include <iostream>
#include "Fen.h"

Fen::Fen(){}

Fen::Fen(string fen){
    int position = 0;

    pieces = "";
    while (fen[position] != ' '){
	pieces.push_back(fen[position]);
	position++;
    }
    position++;


    turn = "";
    while (fen[position] != ' '){
	turn.push_back(fen[position]);
	position++;
    }
    position++;

    castle = "";
    while (fen[position] != ' '){
	castle.push_back(fen[position]);
	position++;
    }
    position++;

    pawn = "";
    while (fen[position] != ' '){
	pawn.push_back(fen[position]);
	position++;
    }
    position++;

    halfmove= "";
    while (fen[position] != ' '){
	halfmove.push_back(fen[position]);
	position++;
    }
    position++;

    fullmove = "";
    while (fen[position] != ' '){
	fullmove.push_back(fen[position]);
	position++;
    }

}

ostream &operator<<(ostream &output, const Fen &F){
    output << F.pieces << " ";
    output << F.turn<< " ";
    output << F.castle << " ";
    output << F.pawn << " ";
    output << F.halfmove << " ";
    output << F.fullmove << " ";
    return output;
}

istream &operator>>(istream &input, Fen &F){
    input >> F.pieces;
    input >> F.turn;
    input >> F.castle;
    input >> F.pawn;
    input >> F.halfmove;
    input >> F.fullmove;
    return input;
}

