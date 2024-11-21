// StockFactory.h
#ifndef STOCK_FACTORY_H
#define STOCK_FACTORY_H

#include "../Observer/Stock.h"

class StockFactory {
public:
    virtual Stock* createStock() = 0;  // Abstract method to create stock
    virtual ~StockFactory() {}
};

#endif // STOCK_FACTORY_H
