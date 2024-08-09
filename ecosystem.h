#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H

const int MAX_TICKS = 100;
const int GRID_SIZE = 10;
const int INITIAL_PLANTS = 30;
const int INITIAL_HERBIVORES = 10;
const int INITIAL_CARNIVORES = 5;

enum CellType { EMPTY, PLANT, HERBIVORE, CARNIVORE };

class Ecosystem {
public:
    Ecosystem();
    void initialize();
    void reproducePlants();
    void update();
    void handlePredation();
    void printGrid() const;

private:
    CellType grid[GRID_SIZE][GRID_SIZE];
    int plantProbability = 30;
    int herbivoreReproductionThreshold = 3;
    int carnivoreReproductionThreshold = 5;
    int herbivoreEnergy = 2;
    int carnivoreEnergy = 2;
    int herbivoreStarvationTicks = 3;
    int carnivoreStarvationTicks = 3;

    void initializeEntities(int count, CellType entityType);
    void moveEntities(CellType entityType);
    void handleStarvation();
    void reproduceHerbivores();
    void reproduceCarnivores();
};

#endif // ECOSYSTEM_H
