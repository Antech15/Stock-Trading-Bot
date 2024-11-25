#ifndef BOT_H
#define BOT_H

#include "stock.h"
#include <QDebug>
#include <QString>
#include <cstdlib> // For rand()


class Bot : public Observer {
public:
    enum class Strategy { Daily, Weekly }; // Declare Strategy enum here

    static Bot &getInstance() { //Apparently this is SINGLETON
        static Bot instance;
        return instance;
    }

    void onPriceUpdate(const QString &stockName, double newPrice) override {
        if(stockCounter <= 0)
        {
            counter--; // Decrement counter
            if (counter <= 0) { // If counter is 0 or negative, make a decision
                    if(day1 == true)
                {
                    int idx = rand() % 4; // Generates a random number between 0 and 3
                    bestStock = stockNames[idx];
                    bestPrice = stockPrices[idx];
                    day1 = false;
                }
                makeDecision(stockName, newPrice); // Make a decision

                // Reset the counter based on the strategy
                if (strategy == Strategy::Daily) {
                    counter = 1; // Make a decision every day
                } else if (strategy == Strategy::Weekly) {
                    counter = 7; // Make a decision every 7 days
                }
            }
            // For weekly strategy, output the number of days left until the bot is active
            if (strategy == Strategy::Weekly && counter > 0) {
                qDebug() << counter << "days left until bot is active on the Stock Market";
            }

            stockCounter = 4;
        }



        stockCounter--;


        stockNames[stockCounter] = stockName;
        stockPrices[stockCounter] = newPrice;



}

    double getBalance() const { return balance; }
    void setStrategy(Strategy newStrategy) { strategy = newStrategy; } // No more error

private:
    Bot() : balance(1000.0), ownedStock(""), purchasePrice(0.0), counter(1), strategy(Strategy::Daily), stockCounter(3) {}

    // Prevent copying
    Bot(const Bot &) = delete;
    void operator=(const Bot &) = delete;

    void makeDecision( QString stockNameThang, double currentPrice) {
        if (ownedStock.isEmpty()) {
            // Buy stock if none is currently owned
            if (balance >= currentPrice) {
                balance -= currentPrice;
                ownedStock = stockNameThang;
                purchasePrice = currentPrice;
                qDebug() << "Bought 1 share of" << stockNameThang << "at $" << currentPrice;
                currentStock = true;
                day1 = false;
            }
        } else  {
            // Check if we should sell
            if (currentPrice > purchasePrice) {
                balance += currentPrice;
                qDebug() << "Sold 1 share of" << stockNameThang << "at $" << currentPrice;
                ownedStock = ""; // No stock owned now
                currentStock = false;
                purchasePrice = 0.0;
            } else {
                qDebug() << "Holding" << stockNameThang << "at $" << currentPrice << ". Purchase price was $" << purchasePrice;
            }
        }
    }

    double balance;
    QString ownedStock;
    double purchasePrice;
    int counter;         // Counter to handle daily/bi-daily logic
    Strategy strategy;   // Current strategy (Daily or Bi-Daily)
    int stockCounter;
    QString stockNames [4];
    int stockPrices[4];
    bool day1 = true;
    double bestDifference;
    QString bestStock;
    double bestPrice;
    bool currentStock = false;
};

#endif // BOT_H
