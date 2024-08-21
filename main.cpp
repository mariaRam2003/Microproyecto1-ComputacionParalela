#include <iostream>
#include "ecosystem.h"

int main() {
    Ecosystem ecosystem;
    ecosystem.initialize();
    
    for (int tick = 0; tick < MAX_TICKS; ++tick) {
        Entity* plants = ecosystem.simulatePlants();
        Entity* herbivores = ecosystem.simulateHerbivores();
        Entity* carnivores = ecosystem.simulateCarnivores();
        ecosystem.mergeGrids(plants, herbivores, carnivores);
        ecosystem.printGrid(tick);
    }
    
    return 0;
}
