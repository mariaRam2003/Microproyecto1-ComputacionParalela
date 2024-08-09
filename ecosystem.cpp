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

void Ecosystem::reproducePlants() {
    Entity tempGrid[GRID_SIZE][GRID_SIZE];

    // We need to copy the grid to a temporary grid to avoid modifying the grid while checking for reproduction
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            tempGrid[i][j] = grid[i][j];        // if values are being copied by reference, the
        }
    }

    // 
    // #pragma omp parallel for collapse(2)
    // // we iterate over the grid to check if a plant can reproduce
    // for (int i = 0; i < GRID_SIZE; ++i) {
    //     for (int j = 0; j < GRID_SIZE; ++j) {

    //         // if the cell is a plant, we check if it can reproduce
    //         if (grid[i][j] == PLANT) {
    //             int reproductionChance = rand() % 100;
    //             if (reproductionChance < plantProbability) {
    //                 for (int di = -1; di <= 1; ++di) {
    //                     for (int dj = -1; dj <= 1; ++dj) {
    //                         int ni = i + di;
    //                         int nj = j + dj;
    //                         if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE && tempGrid[ni][nj] == EMPTY) {
    //                             tempGrid[ni][nj] = PLANT;
    //                             break;
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }

    // TODO: access to grid should be atomic, since multiple threads are writing to it
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = tempGrid[i][j];
        }
    }
}

void Ecosystem::update() {
    moveEntities(HERBIVORE);
    moveEntities(CARNIVORE);
    handleStarvation();
}

void Ecosystem::moveEntities(CellType entityType) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j].type == entityType) {
                // Implementa la lógica de movimiento para la entidad
            }
        }
    }
}

void Ecosystem::handlePredation() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            // Implementa la caza de herbívoros por carnívoros
        }
    }
}

void Ecosystem::handleStarvation() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            // Implementa la lógica de inanición
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