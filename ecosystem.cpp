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

/**
 * @brief 
 * 
 */
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


void Ecosystem::simulateHerbivores() {
    // we check for adyacent cells to see if a herbivore can move
    #pragma omp parallel for collapse(2) shared(grid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {

            // if the cell is not a hervibore, we skip it
            if (grid[i][j].type != HERBIVORE) {
                continue;
            }

            // we check if it starved to death
            if (grid[i][j].satisfaction == 0) {
                Entity empty;
                empty.type = EMPTY;
                grid[i][j] = empty;
                continue;
            }

            // if its youth is 0, it dies of old age
            if (grid[i][j].youth == 0) {
                Entity empty;
                empty.type = EMPTY;
                grid[i][j] = empty;
                continue;
            }

            // we update the satisfaction and youth level
            grid[i][j].satisfaction--;
            grid[i][j].youth--;

            // the directions in which a herbivore can move            
            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

            bool move = false;
            int plant[2] = {0,0}; // this tells us where the plant is relative to us 0,0 means no plant
            bool isPlant = false;

            // we check the adyacent cells
            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0]) % GRID_SIZE;
                int y = (j + directions[k][1]) % GRID_SIZE;
                
                // if there is a carnivore in an adyacent cell we should move to scape
                if (grid[x][y].type == CARNIVORE) {
                    move = true;
                }

                if (grid[x][y].type == PLANT) {
                    plant[0] = x;
                    plant[1] = y;
                    isPlant = true;
                }
            }

            // we check the empty cells now
            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0]) % GRID_SIZE;
                int y = (j + directions[k][1]) % GRID_SIZE;

                if (grid[x][y].type == EMPTY) {
                    // if theres an empty cell and we must move we move
                    if (move) {
                        Entity herbivore_temp = grid[i][j];
                        grid[i][j].type = EMPTY;
                        grid[x][y] = herbivore_temp;
                        break;
                    }else if (isPlant) {
                        // if theres a plant and an empty cell then we reproduce
                        grid[x][y].type = HERBIVORE;
                        grid[x][y].satisfaction = MAX_SATISFACTION;
                        grid[x][y].youth = 5;
                        break;
                    }else if (!isPlant) {
                        // if theres no food then we move 
                        Entity herbivore_temp = grid[i][j];
                        grid[i][j].type = EMPTY;
                        grid[x][y] = herbivore_temp;
                        break;
                    }
                }

                // if theres a plant we eat it
                if (grid[i][j].satisfaction < MAX_SATISFACTION) {
                    grid[i][j].satisfaction++;                            
                }
                grid[plant[0]][plant[1]].type = EMPTY;
                break;
            }
        }   
    }
}

void Ecosystem::simulateCarnivores(){
    #pragma omp parallel for collapse(2) shared(grid)
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            
            // if the cell is not a carnivore, we skip it
            if (grid[i][j].type != CARNIVORE) {
                continue;
            }

            // we check if it starved to death
            if (grid[i][j].satisfaction == 0) {
                Entity empty;
                empty.type = EMPTY;
                grid[i][j] = empty;
                continue;
            }

            // if its youth is 0, it dies of old age
            if (grid[i][j].youth == 0) {
                Entity empty;
                empty.type = EMPTY;
                grid[i][j] = empty;
                continue;
            }

            // we update the satisfaction and youth level
            grid[i][j].satisfaction--;
            grid[i][j].youth--;

            // the directions in which a carnivore can move            
            int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

            bool isHerbivore = false;

            // we check the adyacent cells
            for (int k = 0; k < 4; ++k) {
                int x = (i + directions[k][0]) % GRID_SIZE;
                int y = (j + directions[k][1]) % GRID_SIZE;

                if (grid[x][y].type == HERBIVORE) {
                    isHerbivore = true;
                    break;
                }
            }

            for (int k = 0; k < 4; ++k){
                int x = (i + directions[k][0]) % GRID_SIZE;
                int y = (j + directions[k][1]) % GRID_SIZE;

                // if the cell is NOT empty, we skip it
                if (grid[x][y].type != EMPTY) {
                    continue;
                }

                // if theres an herbivore and an empty cell then we reproduce                    
                if (isHerbivore) {
                    Entity newCarnivore;
                    newCarnivore.type = CARNIVORE;
                    newCarnivore.satisfaction = MAX_SATISFACTION;
                    newCarnivore.youth = 5;
                    grid[x][y] = newCarnivore;
                    break;
                    
                }else if (!isHerbivore) {
                    // if theres no food then we move 
                    Entity carnivore_temp = grid[i][j];
                    grid[i][j].type = EMPTY;
                    grid[x][y] = carnivore_temp;
                    break;
                }

                // if theres a herbivore we eat it
                if (grid[i][j].satisfaction < MAX_SATISFACTION) {
                    grid[i][j].satisfaction++;                            
                }
                grid[x][y].type = EMPTY;
                break;
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
            continue; // if the cell is NOT empty, we try again
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
            grid[x][y] = herbivore;

        } else if (entityType == CARNIVORE) {
            Entity carnivore;
            carnivore.type = CARNIVORE;
            carnivore.satisfaction = MAX_SATISFACTION;
            carnivore.youth = 5;
            carnivore.maxSatisfaction = MAX_SATISFACTION;     
            grid[x][y] = carnivore;

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