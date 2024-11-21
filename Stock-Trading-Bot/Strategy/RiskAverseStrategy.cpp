// RiskAverseStrategy.cpp
#include "InvestmentStrategy.h"
#include <iostream>

class RiskAverseStrategy : public InvestmentStrategy {
public:
    void buyStock() {
        cout << "Buying stocks with a low-risk profile..." << endl;
    }
};
