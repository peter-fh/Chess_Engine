#include <cstdint>
#include "Bithack.h"


Bithack::Bithack(){
    initializeBitPositions();
    loadRays();
}


uint64_t Bithack::getHorizontal(int position){
    return horizontal[position / 8];
}


uint64_t Bithack::getVertical(int position){
    return vertical[position % 8];
}


uint64_t Bithack::getDiagonal(int position){
    int x = position % 8;
    int y = position / 8;
    return diagonal[7-x+y];
}


uint64_t Bithack::getCounterDiagonal(int position){
    int x = position % 8;
    int y = position / 8;
    return counter_diagonal[x+y];
}


uint64_t Bithack::directionalMask(int position, int direction){
    if (direction == POSITIVE){
        uint64_t mask = 0;
        mask = ~mask;
        mask >>= position;
        mask <<= position;
        return mask;
    }
    if (position == 63)
        return ~0ULL;
    uint64_t mask = 0;
    mask = ~mask;
    mask >>= position + 1;
    mask <<= position + 1;
    mask = ~mask;
    return mask;
}


uint64_t Bithack::castRay(int position, int direction){

    if (direction == NORTH){
        return getVertical(position) & directionalMask(position, POSITIVE);
    }

    if (direction == NORTHEAST){
        return getCounterDiagonal(position) & directionalMask(position, POSITIVE);
    }

    if (direction == EAST){
        return getHorizontal(position) & directionalMask(position, NEGATIVE);
    }

    if (direction == SOUTHEAST){
        return getDiagonal(position) & directionalMask(position, NEGATIVE);
    }

    if (direction == SOUTH){
        return getVertical(position) & directionalMask(position, NEGATIVE);
    }

    if (direction == SOUTHWEST){
        return getCounterDiagonal(position) & directionalMask(position, NEGATIVE);
    }

    if (direction == WEST){
        return getHorizontal(position) & directionalMask(position, POSITIVE);
    }

    // else direction == NORTHWEST
    return getDiagonal(position) & directionalMask(position, POSITIVE);

    
}




const uint64_t LEAST_16 =           0x000000000000FFFF;
const uint64_t SECOND_LEAST_16 =    0x00000000FFFF0000;
const uint64_t SECOND_MOST_16 =     0x0000FFFF00000000;
const uint64_t MOST_16 =            0xFFFF000000000000;


int Bithack::leastSignificant(uint64_t piece){

    if (LEAST_16 & piece){
        piece &= LEAST_16;
        return least_significant_positions[piece];
    }

    if (SECOND_LEAST_16 & piece){
        piece &= SECOND_LEAST_16;
        return least_significant_positions[piece >> 16] + 16;
    }

    if (SECOND_MOST_16 & piece){
        piece &= SECOND_MOST_16;
        return least_significant_positions[piece >> 32]+32;
    }

    if (MOST_16 & piece){
        piece &= MOST_16;
        return least_significant_positions[piece >> 48]+48;
    }

    return -1;
    
}


int Bithack::mostSignificant(uint64_t piece){

    if (MOST_16 & piece){
        return most_significant_positions[piece >> 48]+48;
    }

    if (SECOND_MOST_16 & piece){
        return most_significant_positions[piece >> 32]+32;
    }

    if (SECOND_LEAST_16 & piece){
        return most_significant_positions[piece >> 16] + 16;
    }

    if (LEAST_16 & piece){
        return most_significant_positions[piece];
    }

    return -1;
    
}


int Bithack::hammingWeight(uint64_t piece){
    int weight = 0;
    while (piece){
	piece &= ~(1ULL << leastSignificant(piece));
	weight++;
    }

    return weight;
}

void Bithack::initializeBitPositions(){
    for (int i=0; i < 0xffff; i++){
        least_significant_positions[i] = slowLeastSignificant(i);
        most_significant_positions[i] = slowMostSignificant(i);
    }
}


void Bithack::loadRays(){
    uint64_t load_horizontal[8] =  {
                                    0x00000000000000ff,
                                    0x000000000000ff00,
                                    0x0000000000ff0000,
                                    0x00000000ff000000,
                                    0x000000ff00000000,
                                    0x0000ff0000000000,
                                    0x00ff000000000000,
                                    0xff00000000000000
                                    };
    
 
    uint64_t load_vertical[8] =     {
                                0x0101010101010101,
                                0x0202020202020202,
                                0x0404040404040404,
                                0x0808080808080808,
                                0x1010101010101010,
                                0x2020202020202020,
                                0x4040404040404040,
                                0x8080808080808080,
                                    };

    uint64_t load_diagonal[15] =    {
				0x0000000000000080,
                                0x0000000000008040,
                                0x0000000000804020,
                                0x0000000080402010,
                                0x0000008040201008,
                                0x0000804020100804,
                                0x0080402010080402,
                                0x8040201008040201,
                                0x4020100804020100,
                                0x2010080402010000,
                                0x1008040201000000,
                                0x0804020100000000,
                                0x0402010000000000,
                                0x0201000000000000,
                                0x0100000000000000,
                                    };

    uint64_t load_counterdiagonal[15] = {
				    0x0000000000000001,
                                    0x0000000000000102,
                                    0x0000000000010204,
                                    0x0000000001020408,
                                    0x0000000102040810,
                                    0x0000010204081020,
                                    0x0001020408102040,
                                    0x0102040810204080,
                                    0x0204081020408000,
                                    0x0408102040800000,
                                    0x0810204080000000,
                                    0x1020408000000000,
                                    0x2040800000000000,
                                    0x4080000000000000,
                                    0x8000000000000000,
                                        };

    for (int i=0; i < 8; i++){
        horizontal[i] = load_horizontal[i];
        vertical[i] = load_vertical[i];
    }

    for (int i=0; i < 15; i++){
        diagonal[i] = load_diagonal[i];
        counter_diagonal[i] = load_counterdiagonal[i];
    }

}


int Bithack::slowLeastSignificant (int num){
    for (int i=0; i < 16; i++){
        if ((1ULL << i) & num)
            return i;
    }

    return -1;
}


int Bithack::slowMostSignificant (int num){
    for (int i=15; i >= 0; i--){
        if ((1ULL << i) & num)
            return i;
    }

    return -1;
}


