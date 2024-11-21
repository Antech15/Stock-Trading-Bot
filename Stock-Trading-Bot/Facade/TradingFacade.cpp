// TradingFacade.cpp
#include "TradingFacade.h"
#include "../Strategy/InvestmentStrategy.h"
#include "../Factory/StockFactory.h"

TradingFacade::TradingFacade() {
    market = Market::getInstance();
    bot = new TradingBot();

    // Create stocks using factories
    StockFactory* techFactory = new TechStockFactory();
    StockFactory* energyFactory = new EnergyStockFactory();

    stocks.push_back(techFactory->createStock());
    stocks.push_back(energyFactory->createStock());

    // Attach the bot as an observer
    for (Stock* stock : stocks) {
        stock->addObserver(bot);
    }
}

void TradingFacade::performTrade() {
    market->setMarketCapitalization(1000000.0);
    bot->setStrategy(new RiskAverseStrategy());  // Use a strategy for trading
    bot->executeStrategy();
}

void TradingFacade::simulateNextDay() {
    cout << "Simulating next day's trading..." << endl;

    // Simulate price changes for all stocks
    for (Stock* stock : stocks) {
        stock->simulatePriceChange();
    }

    // After price changes, bot may execute more trades
    bot->executeStrategy();
}

TradingFacade::~TradingFacade() {
    delete bot;
    for (Stock* stock : stocks) {
        delete stock;
    }
}
