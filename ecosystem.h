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
    void moveHerbivores();
    void moveCarnivores();
    void printGrid() const;

private:
    CellType grid[GRID_SIZE][GRID_SIZE];
    int plantProbability = 30; // Probabilidad de reproducción de plantas (en porcentaje)
    int herbivoreReproductionThreshold = 3; // Cantidad de plantas necesarias para reproducción de herbívoros
    int carnivoreReproductionThreshold = 5; // Cantidad de herbívoros necesarios para reproducción de carnívoros
    int herbivoreEnergy = 2; // Energía ganada al consumir una planta
    int carnivoreEnergy = 2; // Energía ganada al consumir un herbívoro
    int herbivoreStarvationTicks = 3; // Cantidad de ticks sin comida antes de morir
    int carnivoreStarvationTicks = 3; // Cantidad de ticks sin comida antes de morir

    void reproduceHerbivores();
    void reproduceCarnivores();
    void moveEntities(CellType entityType);
    void handleStarvation();
};

#endif // ECOSYSTEM_H
