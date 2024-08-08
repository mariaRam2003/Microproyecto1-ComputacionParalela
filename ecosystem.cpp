#include <iostream>
#include <omp.h>
#include <cstdlib>
#include "ecosystem.h"

Ecosystem::Ecosystem() {
    // Inicializa la cuadrícula a EMPTY
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = EMPTY;
        }
    }
}

void Ecosystem::initialize() {
    int numPlants = INITIAL_PLANTS;
    int numHerbivores = INITIAL_HERBIVORES;
    int numCarnivores = INITIAL_CARNIVORES;

    while (numPlants > 0 || numHerbivores > 0 || numCarnivores > 0) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;

        if (numPlants > 0 && grid[x][y] == EMPTY) {
            grid[x][y] = PLANT;
            numPlants--;
        } else if (numHerbivores > 0 && grid[x][y] == EMPTY) {
            grid[x][y] = HERBIVORE;
            numHerbivores--;
        } else if (numCarnivores > 0 && grid[x][y] == EMPTY) {
            grid[x][y] = CARNIVORE;
            numCarnivores--;
        }
    }
}

void Ecosystem::reproducePlants() {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == PLANT) {
                // Intentar reproducir plantas en celdas adyacentes
                int reproductionChance = rand() % 100;
                if (reproductionChance < plantProbability) {
                    for (int di = -1; di <= 1; ++di) {
                        for (int dj = -1; dj <= 1; ++dj) {
                            int ni = i + di;
                            int nj = j + dj;
                            if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE && grid[ni][nj] == EMPTY) {
                                grid[ni][nj] = PLANT;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Ecosystem::reproduceHerbivores() {
    // Implementa la lógica de reproducción de herbívoros
}

void Ecosystem::reproduceCarnivores() {
    // Implementa la lógica de reproducción de carnívoros
}

void Ecosystem::update() {
    moveHerbivores();
    moveCarnivores();
    handleStarvation();
}

void Ecosystem::moveHerbivores() {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == HERBIVORE) {
                // Implementa el movimiento de herbívoros
            }
        }
    }
}

void Ecosystem::moveCarnivores() {
    #pragma omp parallel for
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == CARNIVORE) {
                // Implementa el movimiento de carnívoros
            }
        }
    }
}

void Ecosystem::handlePredation() {
    // Implementa la caza de herbívoros por carnívoros
}

void Ecosystem::handleStarvation() {
    // Implementa la lógica de muerte por inanición
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
