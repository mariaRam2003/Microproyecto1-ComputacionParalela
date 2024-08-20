#include <iostream>
#include <omp.h>
#include <cstdlib>
#include "ecosystem.h"

Ecosystem::Ecosystem() {
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        grid[i] = {EMPTY, 0, 0, 0};
        plantGrid[i] = {EMPTY, 0, 0, 0};
        herbivoreGrid[i] = {EMPTY, 0, 0, 0};
        carnivoreGrid[i] = {EMPTY, 0, 0, 0};
    }
}

void Ecosystem::initialize() {
    initializeEntities(INITIAL_PLANTS, PLANT);
    initializeEntities(INITIAL_HERBIVORES, HERBIVORE);
    initializeEntities(INITIAL_CARNIVORES, CARNIVORE);
}

void Ecosystem::simulatePlants() {
    #pragma omp parallel for shared(grid, plantGrid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;

            if (grid[idx].type != PLANT) {
                plantGrid[idx] = grid[idx];
                continue;
            }

            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0] + GRID_SIZE) % GRID_SIZE;
                int y = (j + directions[k][1] + GRID_SIZE) % GRID_SIZE;
                int newIdx = x * GRID_SIZE + y;

                if (plantGrid[newIdx].type == EMPTY) {
                    if (rand() % 100 < 30) {
                        plantGrid[newIdx].type = PLANT;
                    }
                    break;
                }
            }
        }
    }
}

void Ecosystem::simulateHerbivores() {
    #pragma omp parallel for shared(grid, herbivoreGrid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;

            if (grid[idx].type != HERBIVORE) {
                herbivoreGrid[idx] = grid[idx];
                continue;
            }

            if (grid[idx].satisfaction == 0 || grid[idx].youth == 0) {
                herbivoreGrid[idx].type = EMPTY;
                continue;
            }

            herbivoreGrid[idx].satisfaction--;
            herbivoreGrid[idx].youth--;

            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            bool move = false;
            int plant[2] = {0, 0};
            bool isPlant = false;

            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0] + GRID_SIZE) % GRID_SIZE;
                int y = (j + directions[k][1] + GRID_SIZE) % GRID_SIZE;
                int newIdx = x * GRID_SIZE + y;

                if (grid[newIdx].type == CARNIVORE) {
                    move = true;
                }

                if (grid[newIdx].type == PLANT) {
                    plant[0] = x;
                    plant[1] = y;
                    isPlant = true;
                }
            }

            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0] + GRID_SIZE) % GRID_SIZE;
                int y = (j + directions[k][1] + GRID_SIZE) % GRID_SIZE;
                int newIdx = x * GRID_SIZE + y;

                if (herbivoreGrid[newIdx].type == EMPTY) {
                    if (move) {
                        herbivoreGrid[newIdx] = herbivoreGrid[idx];
                        herbivoreGrid[idx].type = EMPTY;
                        break;
                    } else if (isPlant) {
                        herbivoreGrid[newIdx].type = HERBIVORE;
                        herbivoreGrid[newIdx].satisfaction = MAX_SATISFACTION;
                        herbivoreGrid[newIdx].youth = 5;
                        break;
                    } else {
                        herbivoreGrid[newIdx] = herbivoreGrid[idx];
                        herbivoreGrid[idx].type = EMPTY;
                        break;
                    }
                }
            }

            if (herbivoreGrid[idx].satisfaction < MAX_SATISFACTION) {
                herbivoreGrid[idx].satisfaction++;
            }

            if (isPlant) {
                int plantIdx = plant[0] * GRID_SIZE + plant[1];
                herbivoreGrid[plantIdx].type = EMPTY;
            }
        }
    }
}

void Ecosystem::simulateCarnivores() {
    #pragma omp parallel for shared(grid, carnivoreGrid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;

            if (grid[idx].type != CARNIVORE) {
                carnivoreGrid[idx] = grid[idx];
                continue;
            }

            if (grid[idx].satisfaction == 0 || grid[idx].youth == 0) {
                carnivoreGrid[idx].type = EMPTY;
                continue;
            }

            carnivoreGrid[idx].satisfaction--;
            carnivoreGrid[idx].youth--;

            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
            bool move = false;
            int prey[2] = {0, 0};
            bool foundPrey = false;

            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0] + GRID_SIZE) % GRID_SIZE;
                int y = (j + directions[k][1] + GRID_SIZE) % GRID_SIZE;
                int newIdx = x * GRID_SIZE + y;

                if (grid[newIdx].type == HERBIVORE) {
                    prey[0] = x;
                    prey[1] = y;
                    foundPrey = true;
                }
            }

            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0] + GRID_SIZE) % GRID_SIZE;
                int y = (j + directions[k][1] + GRID_SIZE) % GRID_SIZE;
                int newIdx = x * GRID_SIZE + y;

                if (carnivoreGrid[newIdx].type == EMPTY) {
                    if (foundPrey) {
                        carnivoreGrid[newIdx] = carnivoreGrid[idx];
                        carnivoreGrid[newIdx].satisfaction = MAX_SATISFACTION;
                        carnivoreGrid[newIdx].youth = 5;
                        int preyIdx = prey[0] * GRID_SIZE + prey[1];
                        carnivoreGrid[preyIdx].type = EMPTY;
                        break;
                    } else {
                        carnivoreGrid[newIdx] = carnivoreGrid[idx];
                        carnivoreGrid[idx].type = EMPTY;
                        break;
                    }
                }
            }
        }
    }
}

void Ecosystem::mergeGrids() {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        if (carnivoreGrid[i].type == CARNIVORE) {
            grid[i] = carnivoreGrid[i];
        } else if (herbivoreGrid[i].type == HERBIVORE) {
            grid[i] = herbivoreGrid[i];
        } else if (plantGrid[i].type == PLANT) {
            grid[i] = plantGrid[i];
        } else {
            grid[i] = {EMPTY, 0, 0, 0};
        }
    }
}

void Ecosystem::initializeEntities(int count, CellType entityType) {
    int initialized = 0;
    while (initialized < count) {
        int i = rand() % GRID_SIZE;
        int j = rand() % GRID_SIZE;
        int idx = i * GRID_SIZE + j;
        if (grid[idx].type == EMPTY) {
            grid[idx].type = entityType;
            if (entityType == HERBIVORE || entityType == CARNIVORE) {
                grid[idx].satisfaction = MAX_SATISFACTION;
                grid[idx].youth = 5;
            }
            initialized++;
        }
    }
}

void Ecosystem::printGrid(int tick_no) const {
    std::cout << "Tick " << tick_no << ":\n";
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;
            switch (grid[idx].type) {
                case EMPTY:
                    std::cout << ".";
                    break;
                case PLANT:
                    std::cout << "P";
                    break;
                case HERBIVORE:
                    std::cout << "H";
                    break;
                case CARNIVORE:
                    std::cout << "C";
                    break;
            }
        }
        std::cout << "\n";
    }
}
