#include <iostream>
#include <omp.h>
#include <cstdlib>
#include "ecosystem.h"

Ecosystem::Ecosystem() {
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        grid[i] = {EMPTY, 0, 0, 0};
<<<<<<< Updated upstream
=======
        plantGrid[i] = {EMPTY, 0, 0, 0};
        herbivoreGrid[i] = {EMPTY, 0, 0, 0};
        carnivoreGrid[i] = {EMPTY, 0, 0, 0};
>>>>>>> Stashed changes
    }
}

void Ecosystem::initialize() {
    initializeEntities(INITIAL_PLANTS, PLANT);
    initializeEntities(INITIAL_HERBIVORES, HERBIVORE);
    initializeEntities(INITIAL_CARNIVORES, CARNIVORE);
}

<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
                    break;
                }
            }
        }
    }
}

<<<<<<< Updated upstream
void Ecosystem::simulateHerbivores(Entity* tempGrid) {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        tempGrid[i] = grid[i];
        if (grid[i].type == HERBIVORE) {
            if (grid[i].satisfaction == 0 || grid[i].youth == 0) {
                tempGrid[i].type = EMPTY;
=======
void Ecosystem::simulateHerbivores() {
    #pragma omp parallel for shared(grid, herbivoreGrid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;

            if (grid[idx].type != HERBIVORE) {
                herbivoreGrid[idx] = grid[idx];
>>>>>>> Stashed changes
                continue;
            }
            tempGrid[i].satisfaction--;
            tempGrid[i].youth--;

<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
            }
        }
    }
}

<<<<<<< Updated upstream
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
=======
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
>>>>>>> Stashed changes
                        break;
                    }
                }
            }
        }
    }
}

<<<<<<< Updated upstream
void Ecosystem::initializeEntities(int count, CellType entityType) {
    while (count > 0) {
        int idx = rand() % (GRID_SIZE * GRID_SIZE);
        if (grid[idx].type == EMPTY) {
            grid[idx] = {entityType, MAX_SATISFACTION, 5, MAX_SATISFACTION};
            count--;
=======
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
>>>>>>> Stashed changes
        }
    }
}

void Ecosystem::printGrid(int tick_no) const {
    std::cout << "Tick " << tick_no << ":\n";
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int idx = i * GRID_SIZE + j;
            switch (grid[idx].type) {
<<<<<<< Updated upstream
                case PLANT: std::cout << "P "; break;
                case HERBIVORE: std::cout << "H "; break;
                case CARNIVORE: std::cout << "C "; break;
                default: std::cout << ". "; break;
=======
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
>>>>>>> Stashed changes
            }
        }
        std::cout << "\n";
    }
}
