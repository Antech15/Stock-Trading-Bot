// TradingFacade.h
#ifndef TRADING_FACADE_H
#define TRADING_FACADE_H

#include "../TradingBot.h"
#include "../Singleton/Market.h"
#include "../Observer/Stock.h"
#include "../Factory/StockFactory.h"
#include <vector>

class TradingFacade {
private:
    Market* market;
    TradingBot* bot;
    std::vector<Stock*> stocks;

public:
    TradingFacade();
    void performTrade();
    void simulateNextDay();
    ~TradingFacade();
};

#endif // TRADING_FACADE_H
