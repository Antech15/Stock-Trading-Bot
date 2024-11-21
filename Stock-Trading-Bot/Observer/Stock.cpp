// Stock.cpp
#include "Stock.h"
#include <iostream>
#include <vector>

// Notify all observers when the stock price changes
void Stock::notifyObservers() {
    for (Observer* observer : observers) {
        observer->update();  // Notify each observer (e.g., TradingBot)
    }
}

// Add an observer (TradingBot) to the list
void Stock::addObserver(Observer* observer) {
    observers.push_back(observer);
}

// Remove an observer (if needed, e.g., for cleanup)
// This will manually find and remove the observer from the list
void Stock::removeObserver(Observer* observer) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == observer) {
            observers.erase(it);
            break;  // Exit after finding the observer to remove
        }
    }
}

// Set the price of the stock and notify observers
void Stock::setPrice(float newPrice) {
    price = newPrice;
    notifyObservers();  // Notify all observers when price changes
}

// Get the current price of the stock
float Stock::getPrice() const {
    return price;
}

// Simulate a price change (for example, random fluctuation)
void Stock::simulatePriceChange() {
    price += rand() % 10 - 5;  // Random price change between -5 and +5
    notifyObservers();  // Notify observers after price change
}
