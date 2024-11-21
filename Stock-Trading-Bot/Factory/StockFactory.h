#ifndef STOCKFACTORY_H
#define STOCKFACTORY_H

#include "../Stock/Stock.h"

class StockFactory {
public:
    virtual Stock* createStock() = 0;  // Pure virtual method to create stock
};

#endif
