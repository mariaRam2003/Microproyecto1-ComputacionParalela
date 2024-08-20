#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H

const int MAX_TICKS = 10;
const int GRID_SIZE = 10;
const int INITIAL_PLANTS = 5;
const int INITIAL_HERBIVORES = 10;
const int INITIAL_CARNIVORES = 5;
const int MAX_SATISFACTION = 5;

enum CellType { EMPTY, PLANT, HERBIVORE, CARNIVORE };

struct Entity {
    CellType type;
    int satisfaction;
    int youth;
    int maxSatisfaction;
};

class Ecosystem {
public:
    Ecosystem();
    void initialize();    
    void printGrid(int tick_no) const;
    void simulatePlants();
    void simulateHerbivores();
    void simulateCarnivores();
    void mergeGrids();  // Nuevo método para combinar los grids temporales

private:
    Entity grid[GRID_SIZE * GRID_SIZE];
    Entity plantGrid[GRID_SIZE * GRID_SIZE];
    Entity herbivoreGrid[GRID_SIZE * GRID_SIZE];
    Entity carnivoreGrid[GRID_SIZE * GRID_SIZE];

    bool chance(int probability);
    void initializeEntities(int count, CellType entityType);
};

#endif // ECOSYSTEM_H