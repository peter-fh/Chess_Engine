#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>


class Bithack
{
public:
    Bithack();

    // Ray functions
    uint64_t getHorizontal(int position);
    uint64_t getVertical(int position);
    uint64_t getDiagonal(int position);
    uint64_t getCounterDiagonal(int position);
    uint64_t directionalMask(int position, int direction);
    uint64_t castRay(int position, int direction);

    // Bit functions
    int leastSignificant(uint64_t piece);
    int mostSignificant(uint64_t piece);

private:

    int64_t horizontal[8];
    uint64_t vertical[8];
    uint64_t diagonal[15];
    uint64_t counter_diagonal[15];
    uint64_t least_significant_positions[65536];
    uint64_t most_significant_positions[65536];

    void initializeBitPositions();
    void loadRays();   
    int slowLeastSignificant(int piece);
    int slowMostSignificant(int piece);
};


const int POSITIVE = 1;
const int NEGATIVE = -1;

// Bitshift left by this amount travels one square in this direction:
const int WEST = 1;
const int NORTHWEST = 9;
const int NORTH = 8;
const int NORTHEAST = 7;
const int EAST = -1;
const int SOUTHEAST = -9;
const int SOUTH = -8;
const int SOUTHWEST = -7;


