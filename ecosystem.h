#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H

const int MAX_TICKS = 1000;
const int GRID_SIZE = 10;
const int INITIAL_PLANTS = 5;
const int INITIAL_HERBIVORES = 10;
const int INITIAL_CARNIVORES = 5;
const int MAX_SATISFACTION = 5;

enum CellType { EMPTY, PLANT, HERBIVORE, CARNIVORE };

/**
 * @brief Struct to represent an entity in the ecosystem i.e a plant, herbivore or carnivore
 * 
 * @param type The type of the entity i.e a plant, herbivore or carnivore
 * @param satisfaction The satisfaction level of the entity, when it reaches 0, the entity dies of starvation
 * @param youth when the youth level of the entity reaches 0, the entity dies of old age
 * @param maxSatisfaction The entity can keep eating but the satisfaction level cannot exceed this value
 */
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
    void printGrid(int tick_no);
    Entity* simulatePlants();
    Entity* simulateHerbivores();
    Entity* simulateCarnivores();
    void mergeGrids(Entity* plants, Entity* herbivores, Entity* carnivores);

private:
    Entity grid[GRID_SIZE * GRID_SIZE];
    int converter(int x, int y);
    bool chance(int probability);
    Entity* copier(Entity* grid);
    int plantProbability = 15;
    int herbivoreReproductionThreshold = 3;
    int carnivoreReproductionThreshold = 5;
    int herbivoreEnergy = 2;
    int carnivoreEnergy = 2;
    int herbivoreStarvationTicks = 3;
    int carnivoreStarvationTicks = 3;

    void initializeEntities(int count, CellType entityType);
};

#endif // ECOSYSTEM_H
