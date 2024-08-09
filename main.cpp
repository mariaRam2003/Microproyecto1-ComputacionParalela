#include <iostream>
#include "ecosystem.h"

int main() {
    Ecosystem ecosystem;
    ecosystem.initialize();
    
    for (int tick = 0; tick < MAX_TICKS; ++tick) {
        ecosystem.simulatePlants();
        ecosystem.printGrid(tick);
    }
    
    return 0;
}
