// GrowthStrategy.cpp
#include "InvestmentStrategy.h"
#include <iostream>

class GrowthStrategy : public InvestmentStrategy {
public:
    void buyStock() {
        cout << "Buying stocks with high growth potential..." << endl;
    }
};
