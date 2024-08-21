#include <iostream>
#include <omp.h>
#include "ecosystem.h"

int main() {
    Ecosystem ecosystem;
    ecosystem.initialize();

    // Set the number of threads for OpenMP (optional, defaults to available threads)
    omp_set_num_threads(3); // Adjust based on the number of parallelizable tasks

    for (int tick = 0; tick < MAX_TICKS; ++tick) {
        Entity *plants, *herbivores, *carnivores;

        // Parallelize the simulation methods
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                plants = ecosystem.simulatePlants();
            }

            #pragma omp section
            {
                herbivores = ecosystem.simulateHerbivores();
            }

            #pragma omp section
            {
                carnivores = ecosystem.simulateCarnivores();
            }
        }

        // Ensure all simulation methods have completed before proceeding
        #pragma omp barrier

        // Merge grids and print the grid after all simulations are complete
        ecosystem.mergeGrids(plants, herbivores, carnivores);
        ecosystem.printGrid(tick);
    }

    return 0;
}