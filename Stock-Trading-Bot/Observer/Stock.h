// Stock.h
#ifndef STOCK_H
#define STOCK_H

#include "Observer.h"
#include <vector>

class Stock {
private:
    float price;
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notifyObservers();
    void setPrice(float newPrice);
    float getPrice() const;
    void simulatePriceChange();
};

#endif // STOCK_H
