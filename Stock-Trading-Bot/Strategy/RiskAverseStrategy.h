#ifndef RISKAVERSESTRATEGY_H
#define RISKAVERSESTRATEGY_H

#include "TradingStrategy.h"
#include <iostream>

class RiskAverseStrategy : public TradingStrategy {
public:
    void execute() {
        std::cout << "Buying stocks with a low-risk profile..." << std::endl;
    }
};

#endif
