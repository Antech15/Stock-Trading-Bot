// InvestmentStrategy.h
#ifndef INVESTMENT_STRATEGY_H
#define INVESTMENT_STRATEGY_H

class InvestmentStrategy {
public:
    virtual void buyStock() = 0;  // Pure virtual method to buy stocks
    virtual ~InvestmentStrategy() {}
};

#endif // INVESTMENT_STRATEGY_H
