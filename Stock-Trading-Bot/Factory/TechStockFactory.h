#ifndef TECHSTOCKFACTORY_H
#define TECHSTOCKFACTORY_H

#include "StockFactory.h"
#include "../Stock/TechStock.h"

class TechStockFactory : public StockFactory {
public:
    Stock* createStock() {
        return new TechStock("TechCorp", 100.0);  // Return a new TechStock instance
    }
};

#endif
