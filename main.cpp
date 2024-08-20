#include <iostream>
#include "ecosystem.h"

int main() {
    Ecosystem ecosystem;
    ecosystem.initialize();
    
    for (int tick = 0; tick < MAX_TICKS; ++tick) {
        std::cout << "Starting Tick " << tick << std::endl;
        ecosystem.simulatePlants();
        ecosystem.simulateHerbivores();
        ecosystem.simulateCarnivores();
        ecosystem.printGrid(tick);
        std::cout << "Ending Tick " << tick << std::endl;
    }
    
    return 0;
}
