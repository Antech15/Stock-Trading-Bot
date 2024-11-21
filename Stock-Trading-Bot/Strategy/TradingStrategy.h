#ifndef TRADINGSTRATEGY_H
#define TRADINGSTRATEGY_H

class TradingStrategy {
public:
    virtual void execute() = 0;  // Pure virtual method to be implemented by specific strategies
};

#endif
