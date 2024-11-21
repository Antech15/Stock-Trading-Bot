#ifndef TRADINGBOT_H
#define TRADINGBOT_H

#include "Observer/Observer.h"
#include "Strategy/TradingStrategy.h"
#include <iostream>

class TradingBot : public Observer {
private:
    TradingStrategy* strategy;

public:
    void setStrategy(TradingStrategy* newStrategy) {
        strategy = newStrategy;
    }

    void update() {
        if (strategy) {
            std::cout << "Executing trading strategy..." << std::endl;
            strategy->execute();
        } else {
            std::cout << "No strategy set." << std::endl;
        }
    }
};

#endif
