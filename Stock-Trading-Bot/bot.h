#ifndef BOT_H
#define BOT_H

#include "stock.h"
#include <QDebug>
#include <QString>

class Bot : public Observer {
public:
    enum class Strategy { Daily, BiDaily }; // Declare Strategy enum here

    static Bot &getInstance() { //Apparently this is SINGLETON
        static Bot instance;
        return instance;
    }

    void onPriceUpdate(const QString &stockName, double newPrice) override {
        if (--counter <= 0) {
            counter = (strategy == Strategy::Daily) ? 1 : 2; // Reset counter based on strategy
            makeDecision(stockName, newPrice);
        }
    }

    double getBalance() const { return balance; }
    void setStrategy(Strategy newStrategy) { strategy = newStrategy; } // No more error

private:
    Bot() : balance(1000.0), ownedStock(""), purchasePrice(0.0), counter(1), strategy(Strategy::Daily) {}

    // Prevent copying
    Bot(const Bot &) = delete;
    void operator=(const Bot &) = delete;

    void makeDecision(const QString &stockName, double currentPrice) {
        if (ownedStock.isEmpty()) {
            // Buy stock if none is currently owned
            if (balance >= currentPrice) {
                balance -= currentPrice;
                ownedStock = stockName;
                purchasePrice = currentPrice;
                qDebug() << "Bought 1 share of" << stockName << "at $" << currentPrice;
            }
        } else if (ownedStock == stockName) {
            // Check if we should sell
            if (currentPrice > purchasePrice) {
                balance += currentPrice;
                qDebug() << "Sold 1 share of" << stockName << "at $" << currentPrice;
                ownedStock = ""; // No stock owned now
                purchasePrice = 0.0;
            } else {
                qDebug() << "Holding" << stockName << "at $" << currentPrice << ". Purchase price was $" << purchasePrice;
            }
        }
    }

    double balance;
    QString ownedStock;
    double purchasePrice;
    int counter;         // Counter to handle daily/bi-daily logic
    Strategy strategy;   // Current strategy (Daily or Bi-Daily)
};

#endif // BOT_H
