#include <iostream>
#include "Fen.h"

Fen::Fen(){}

Fen::Fen(string fen){
    int position = 0;
    int len = fen.length();

    pieces = "";
    while (fen[position] != ' ' && position < len){
	pieces.push_back(fen[position]);
	position++;
    }
    position++;


    turn = "";
    while (fen[position] != ' '&& position < len){
	turn.push_back(fen[position]);
	position++;
    }
    position++;

    castle = "";
    while (fen[position] != ' '&& position < len){
	castle.push_back(fen[position]);
	position++;
    }
    position++;

    pawn = "";
    while (fen[position] != ' '&& position < len){
	pawn.push_back(fen[position]);
	position++;
    }
    position++;

    halfmove= "";
    while (fen[position] != ' '&& position < len){
	halfmove.push_back(fen[position]);
	position++;
    }
    position++;

    fullmove = "";
    while (fen[position] != ' '&& position < len){
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

