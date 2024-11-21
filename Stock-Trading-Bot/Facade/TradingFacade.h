#ifndef TRADINGFACADE_H
#define TRADINGFACADE_H

#include "../Factory/StockFactory.h"
#include "../Factory/TechStockFactory.h"
#include "../Factory/EnergyStockFactory.h"
#include "../Singleton/Market.h"
#include "../Observer/Observer.h"
#include "../TradingBot.h"
#include <iostream>

class TradingFacade {
public:
    void simulateNextDay() {
        // Initialize stock factories
        StockFactory* techFactory = new TechStockFactory();
        StockFactory* energyFactory = new EnergyStockFactory();

        // Create stocks
        Stock* techStock = techFactory->createStock();
        Stock* energyStock = energyFactory->createStock();

        // Add stocks to the market
        Market* market = Market::getInstance();
        market->addStock(techStock);
        market->addStock(energyStock);

        // Notify observers (e.g., TradingBot)
        std::cout << "Simulating next day's trading..." << std::endl;
    }
};

#endif
