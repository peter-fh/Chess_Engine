#include <cstdint>
#include <map>
#include <stdint.h>
#include <stdbool.h>
#include <string>



class Bithack{
    uint64_t horizontal[8];
    uint64_t vertical[8];
    uint64_t diagonal[15];
    uint64_t counter_diagonal[15];
    uint64_t least_significant_positions[65536];
    uint64_t most_significant_positions[65536];
    
    int slowLeastSignificant(int piece);
    int slowMostSignificant(int piece);


    public:
        Bithack(){
            initializeBitPositions();
            loadRays();
        }
        void initializeBitPositions();
        void loadRays();
        uint64_t getHorizontal(int position);
        uint64_t getVertical(int position);
        uint64_t getDiagonal(int position);
        uint64_t getCounterDiagonal(int position);
        uint64_t directionalMask(int position, int direction);
        uint64_t castRay(int position, int direction);
        int leastSignificant(uint64_t piece);
        int mostSignificant(uint64_t piece);
};

