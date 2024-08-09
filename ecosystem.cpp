#include <iostream>
#include <omp.h>
#include <cstdlib>
#include "ecosystem.h"

Ecosystem::Ecosystem() {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = EMPTY;
        }
    }
}

void Ecosystem::initialize() {
    initializeEntities(INITIAL_PLANTS, PLANT);
    initializeEntities(INITIAL_HERBIVORES, HERBIVORE);
    initializeEntities(INITIAL_CARNIVORES, CARNIVORE);
}

void Ecosystem::initializeEntities(int count, CellType entityType) {
    while (count > 0) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;

        if (grid[x][y] == EMPTY) {
            grid[x][y] = entityType;
            count--;
        }
    }
}

void Ecosystem::reproducePlants() {
    CellType tempGrid[GRID_SIZE][GRID_SIZE];

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            tempGrid[i][j] = grid[i][j];
        }
    }

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == PLANT) {
                int reproductionChance = rand() % 100;
                if (reproductionChance < plantProbability) {
                    for (int di = -1; di <= 1; ++di) {
                        for (int dj = -1; dj <= 1; ++dj) {
                            int ni = i + di;
                            int nj = j + dj;
                            if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE && tempGrid[ni][nj] == EMPTY) {
                                tempGrid[ni][nj] = PLANT;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

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
            if (grid[i][j] == entityType) {
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

void Ecosystem::printGrid() const {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            switch (grid[i][j]) {
                case PLANT: std::cout << "P "; break;
                case HERBIVORE: std::cout << "H "; break;
                case CARNIVORE: std::cout << "C "; break;
                default: std::cout << ". "; break;
            }
        }
        std::cout << std::endl;
    }
}