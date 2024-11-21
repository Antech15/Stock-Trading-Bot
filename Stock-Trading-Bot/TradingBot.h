// TradingBot.h
#ifndef TRADING_BOT_H
#define TRADING_BOT_H

#include "Strategy/InvestmentStrategy.h"

class TradingBot {
private:
    InvestmentStrategy* strategy;

public:
    TradingBot();
    void setStrategy(InvestmentStrategy* newStrategy);
    void executeStrategy();
    ~TradingBot();
};

#endif // TRADING_BOT_H
