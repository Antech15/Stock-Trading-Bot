#ifndef STOCK_H
#define STOCK_H

#include <QString>
#include <QtCore/qdebug.h>
#include <vector>
#include <memory>
#include <cstdlib>

class Observer {
public:
    virtual void onPriceUpdate(const QString &stockName, double newPrice) = 0;
};

class Stock {
public:
    Stock(const QString &name, double price) : name(name), price(price) {}

    void attach(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    void notify() {
        for (auto &observer : observers) {
            observer->onPriceUpdate(name, price);
        }
    }

    void updatePrice() {
        price += (rand() % 200 - 100) / 100.0; // Random price change
        if (price < 0) price = 0;              // Prevent negative price
        notify();
    }

    QString getName() const { return name; }
    double getPrice() const { return price; }
private:
    QString name;
    double price;
    std::vector<std::shared_ptr<Observer>> observers;
};

#endif // STOCK_H
