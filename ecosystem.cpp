#include <iostream>
#include <omp.h>
#include <cstdlib>
#include "ecosystem.h"

Ecosystem::Ecosystem() {
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        grid[i] = {EMPTY, 0, 0, 0};
    }
}

void Ecosystem::initialize() {
    initializeEntities(INITIAL_PLANTS, PLANT);
    initializeEntities(INITIAL_HERBIVORES, HERBIVORE);
    initializeEntities(INITIAL_CARNIVORES, CARNIVORE);
}

void Ecosystem::simulate() {
    Entity tempGrid[GRID_SIZE * GRID_SIZE];

    #pragma omp parallel sections
    {
        #pragma omp section
        simulatePlants(tempGrid);

        #pragma omp section
        simulateHerbivores(tempGrid);

        #pragma omp section
        simulateCarnivores(tempGrid);
    }

    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        grid[i] = tempGrid[i];
    }
}

void Ecosystem::simulatePlants(Entity* tempGrid) {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        tempGrid[i] = grid[i];
        if (grid[i].type == PLANT) {
            int directions[4] = {-1, 1, -GRID_SIZE, GRID_SIZE};
            for (int k = 0; k < 4; ++k) {
                int newIdx = (i + directions[k] + GRID_SIZE * GRID_SIZE) % (GRID_SIZE * GRID_SIZE);
                if (tempGrid[newIdx].type == EMPTY && rand() % 100 < 30) {
                    tempGrid[newIdx].type = PLANT;
                    break;
                }
            }
        }
    }
}

void Ecosystem::simulateHerbivores(Entity* tempGrid) {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        tempGrid[i] = grid[i];
        if (grid[i].type == HERBIVORE) {
            if (grid[i].satisfaction == 0 || grid[i].youth == 0) {
                tempGrid[i].type = EMPTY;
                continue;
            }
            tempGrid[i].satisfaction--;
            tempGrid[i].youth--;

            int directions[4] = {-1, 1, -GRID_SIZE, GRID_SIZE};
            bool moved = false;
            for (int k = 0; k < 4; ++k) {
                int newIdx = (i + directions[k] + GRID_SIZE * GRID_SIZE) % (GRID_SIZE * GRID_SIZE);
                if (grid[newIdx].type == PLANT) {
                    tempGrid[newIdx].type = HERBIVORE;
                    tempGrid[newIdx].satisfaction = MAX_SATISFACTION;
                    tempGrid[newIdx].youth = 5;
                    tempGrid[i].type = EMPTY;
                    moved = true;
                    break;
                }
            }
            if (!moved) {
                for (int k = 0; k < 4; ++k) {
                    int newIdx = (i + directions[k] + GRID_SIZE * GRID_SIZE) % (GRID_SIZE * GRID_SIZE);
                    if (tempGrid[newIdx].type == EMPTY) {
                        tempGrid[newIdx] = tempGrid[i];
                        tempGrid[i].type = EMPTY;
                        break;
                    }
                }
            }
        }
    }
}

void Ecosystem::simulateCarnivores(Entity* tempGrid) {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        tempGrid[i] = grid[i];
        if (grid[i].type == CARNIVORE) {
            if (grid[i].satisfaction == 0 || grid[i].youth == 0) {
                tempGrid[i].type = EMPTY;
                continue;
            }
            tempGrid[i].satisfaction--;
            tempGrid[i].youth--;

            int directions[4] = {-1, 1, -GRID_SIZE, GRID_SIZE};
            bool moved = false;
            for (int k = 0; k < 4; ++k) {
                int newIdx = (i + directions[k] + GRID_SIZE * GRID_SIZE) % (GRID_SIZE * GRID_SIZE);
                if (grid[newIdx].type == HERBIVORE) {
                    tempGrid[newIdx].type = CARNIVORE;
                    tempGrid[newIdx].satisfaction = MAX_SATISFACTION;
                    tempGrid[newIdx].youth = 5;
                    tempGrid[i].type = EMPTY;
                    moved = true;
                    break;
                }
            }
            if (!moved) {
                for (int k = 0; k < 4; ++k) {
                    int newIdx = (i + directions[k] + GRID_SIZE * GRID_SIZE) % (GRID_SIZE * GRID_SIZE);
                    if (tempGrid[newIdx].type == EMPTY) {
                        tempGrid[newIdx] = tempGrid[i];
                        tempGrid[i].type = EMPTY;
                        break;
                    }
                }
            }
        }
    }
}

void Ecosystem::initializeEntities(int count, CellType entityType) {
    while (count > 0) {
        int idx = rand() % (GRID_SIZE * GRID_SIZE);
        if (grid[idx].type == EMPTY) {
            grid[idx] = {entityType, MAX_SATISFACTION, 5, MAX_SATISFACTION};
            count--;
        }
    }
}

void Ecosystem::printGrid(int tick_no) const {
    std::cout << std::endl;
    std::cout << "Tick " << tick_no << std::endl;

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;
            switch (grid[idx].type) {
                case PLANT: std::cout << "P "; break;
                case HERBIVORE: std::cout << "H "; break;
                case CARNIVORE: std::cout << "C "; break;
                default: std::cout << ". "; break;
            }
        }
        std::cout << std::endl;
    }
}
