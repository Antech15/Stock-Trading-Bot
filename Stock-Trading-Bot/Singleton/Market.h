#ifndef MARKET_H
#define MARKET_H

#include <vector>
#include "../Stock/Stock.h"

class Market {
private:
    static Market* instance;
    std::vector<Stock*> stocks;

    Market() {}  // Private constructor to prevent instantiation outside the class

public:
    static Market* getInstance() {
        if (!instance)
            instance = new Market();  // Only create the instance once
        return instance;
    }

    void addStock(Stock* stock) {
        stocks.push_back(stock);  // Add a stock to the market
    }

    float getMarketCapitalization() const {
        float total = 0;
        for (auto stock : stocks) {
            total += stock->getPrice();  // Sum the price of all stocks
        }
        return total;
    }
};

// Initialize the static instance to nullptr
Market* Market::instance = nullptr;

#endif
