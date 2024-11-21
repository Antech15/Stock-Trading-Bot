#ifndef ENERGYSTOCK_H
#define ENERGYSTOCK_H

#include "Stock.h"

class EnergyStock : public Stock {
public:
    EnergyStock(std::string name, float price) : Stock(name, price) {}

    // Custom methods specific to EnergyStock could be added here
};

#endif
