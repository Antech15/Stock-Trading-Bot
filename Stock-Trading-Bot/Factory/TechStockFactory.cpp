// TechStockFactory.cpp
#include "StockFactory.h"
#include "../Observer/Stock.h"

class TechStockFactory : public StockFactory {
public:
    Stock* createStock() {
        Stock* stock = new Stock();
        stock->setPrice(100);  // Tech stock starts with price 100
        return stock;
    }
};
