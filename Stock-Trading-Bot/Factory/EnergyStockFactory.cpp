// EnergyStockFactory.cpp
#include "StockFactory.h"
#include "../Observer/Stock.h"

class EnergyStockFactory : public StockFactory {
public:
    Stock* createStock() {
        Stock* stock = new Stock();
        stock->setPrice(50);  // Energy stock starts with price 50
        return stock;
    }
};
