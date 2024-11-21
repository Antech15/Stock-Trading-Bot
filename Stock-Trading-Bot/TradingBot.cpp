// TradingBot.cpp
#include "TradingBot.h"

TradingBot::TradingBot() : strategy(nullptr) {}

void TradingBot::setStrategy(InvestmentStrategy* newStrategy) {
    strategy = newStrategy;
}

void TradingBot::executeStrategy() {
    if (strategy) {
        cout << "Executing trading strategy..." << endl;
        strategy->buyStock();  // Execute the strategy's buyStock method
    } else {
        cout << "No strategy set." << endl;
    }
}

TradingBot::~TradingBot() {
    delete strategy;  // Clean up the strategy (if set)
}
