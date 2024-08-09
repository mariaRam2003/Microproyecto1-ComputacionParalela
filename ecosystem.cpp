#include <iostream>
#include <omp.h>
#include <cstdlib>
#include "ecosystem.h"

Ecosystem::Ecosystem() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            Entity empty;
            empty.type = EMPTY;
            grid[i][j] = empty;
        }
    }
}

void Ecosystem::initialize() {
    initializeEntities(INITIAL_PLANTS, PLANT);
    initializeEntities(INITIAL_HERBIVORES, HERBIVORE);
    initializeEntities(INITIAL_CARNIVORES, CARNIVORE);
}

void Ecosystem::simulatePlants(){
    // we check for adyacent cells to see if a plant can grow
    // TODO: the shared grid could introduce serious overhead and cache invalidation, we should consider a better approach
    #pragma omp parallel for collapse(2) shared(grid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {

            // if the cell is not a plant, we skip it
            if (grid[i][j].type != PLANT) {
                continue;
            }

            // the directions in which a plant can grow
            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0]) % GRID_SIZE;
                int y = (j + directions[k][1]) % GRID_SIZE;
                
                // if the cell is not empty, we skip it
                if (grid[x][y].type != EMPTY) {
                        continue;
                }

                // if the cell is empty, we check if a plant can grow
                if (chance(plantProbability)) {
                        Entity plant;
                        plant.type = PLANT;
                        grid[x][y] = plant;
                }
            }

        }
    }
}

bool Ecosystem::chance(int probability) {
    return (rand() % 100) < probability;
}


void Ecosystem::initializeEntities(int count, CellType entityType) {
    for (;count > 0; count--) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;        

        if (grid[x][y].type != EMPTY) {
            continue; // if the cell is not empty, we try again
        }        

        if (entityType == PLANT) {
            Entity plant;
            plant.type = PLANT;
            grid[x][y] = plant;

        } else if (entityType == HERBIVORE) {
            Entity herbivore;
            herbivore.type = HERBIVORE;
            herbivore.satisfaction = MAX_SATISFACTION;
            herbivore.youth = 5;
            herbivore.maxSatisfaction = MAX_SATISFACTION;

        } else if (entityType == CARNIVORE) {
            Entity carnivore;
            carnivore.type = CARNIVORE;
            carnivore.satisfaction = MAX_SATISFACTION;
            carnivore.youth = 5;
            carnivore.maxSatisfaction = MAX_SATISFACTION;            
        }

    }
}


void Ecosystem::printGrid(int tick_no) const {
    std::cout << std::endl;
    std::cout << "Tick " << tick_no << std::endl;

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            switch (grid[i][j].type) {
                case PLANT: std::cout << "P "; break;
                case HERBIVORE: std::cout << "H "; break;
                case CARNIVORE: std::cout << "C "; break;
                default: std::cout << ". "; break;
            }
        }
        std::cout << std::endl;
    }
}