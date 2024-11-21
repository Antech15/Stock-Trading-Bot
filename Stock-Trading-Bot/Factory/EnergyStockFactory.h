#ifndef ENERGYSTOCKFACTORY_H
#define ENERGYSTOCKFACTORY_H

#include "StockFactory.h"
#include "../Stock/EnergyStock.h"

class EnergyStockFactory : public StockFactory {
public:
    Stock* createStock() {
        return new EnergyStock("EnergyCorp", 50.0);  // Return a new EnergyStock instance
    }
};

#endif
