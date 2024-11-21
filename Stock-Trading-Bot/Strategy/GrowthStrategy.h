#ifndef GROWTHSTRATEGY_H
#define GROWTHSTRATEGY_H

#include "TradingStrategy.h"
#include <iostream>

class GrowthStrategy : public TradingStrategy {
public:
    void execute() {
        std::cout << "Buying stocks with high growth potential..." << std::endl;
    }
};

#endif
