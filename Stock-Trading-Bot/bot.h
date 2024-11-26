#ifndef BOT_H
#define BOT_H

#include "stock.h"
#include <QDebug>
#include <QString>
#include <cstdlib> // For rand()

class BotStrategy {
public:
    virtual void onPriceUpdate(const QString &stockName, double newPrice) = 0;
};

class Bot : public Observer {
public:
    static Bot &getInstance() { //Apparently this is SINGLETON
        static Bot instance;
        return instance;
    }

    void onPriceUpdate(const QString &stockName, double newPrice) override {
        if(strategy_ != nullptr){
           return strategy_->onPriceUpdate(stockName, newPrice);
        }
        return void();

}

    void deposit(double num) {
        balance += num;
    }

    QString getBotBubble() {
        QString temp = "";
        if(ownedStock != "")
            temp = "I currently own " + ownedStock + " at " + QString::number(ownedStockPrice) + "!!";
        else
            temp = "Which one should I buy next??";
        return temp;

    }


    double getBalance() const { return balance; }
    void setStrategy(BotStrategy *strategy) { strategy_ = strategy; } // No more error


    Bot() : balance(1000.0), ownedStock(""), purchasePrice(0.0), counter(1), stockCounter(3), strategy_(nullptr) {}

    // Prevent copying
    Bot(const Bot &) = delete;
    void operator=(const Bot &) = delete;

    void makeDecision( QString stockNameThang, double currentPrice) {
        if (ownedStock.isEmpty()) {
            // Buy stock if none is currently owned
            if (balance >= currentPrice) {
                balance -= currentPrice;
                ownedStock = stockNameThang;
                ownedStockPrice = currentPrice;
                purchasePrice = currentPrice;
                qDebug() << "Bought 1 share of" << stockNameThang << "at $" << currentPrice;
                currentStock = true;
                bestChange = 2;
                day1 = false;
            }
        } else  {
            // Check if we should sell
            if (currentPrice > purchasePrice) {
                balance += currentPrice;
                qDebug() << "Sold 1 share of" << stockNameThang << "at $" << currentPrice;
                bestChange = 2;
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
    int counter;
    int stockCounter;
    QString stockNames [4];
    double stockPrices[4];
    bool day1 = true;
    double bestDifference;
    QString bestStock;
    double bestPrice;
    bool currentStock = false;
    double bestChange = 2;
    double ownedStockPrice;
    BotStrategy* strategy_;
};

class Daily : public BotStrategy {
public:
    Daily(){}

    void onPriceUpdate(const QString &stockName, double newPrice)
    {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance

        {
            if(botInstance.day1){
                botInstance.stockNames[botInstance.stockCounter] = stockName;
                botInstance.stockPrices[botInstance.stockCounter] = newPrice;}
            else{
                qDebug() << stockName << " | stockCounter: " << botInstance.stockCounter << " " << newPrice << " / " << botInstance.stockPrices[botInstance.stockCounter] << " = " << newPrice/botInstance.stockPrices[botInstance.stockCounter];

                if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange){
                    botInstance.bestChange = newPrice/botInstance.stockPrices[botInstance.stockCounter];
                    botInstance.bestStock = stockName;
                    botInstance.bestPrice = newPrice;
                }
                botInstance.stockNames[botInstance.stockCounter] = stockName;
                botInstance.stockPrices[botInstance.stockCounter] = newPrice;
                if(stockName == botInstance.ownedStock)
                {
                    botInstance.ownedStockPrice = newPrice;
                }
            }
            if(botInstance.stockCounter <= 0)
            {
                botInstance.counter--; // Decrement counter
                if (botInstance.counter <= 0) { // If counter is 0 or negative, make a decision
                    if(botInstance.day1 == true)
                    {
                        int idx = rand() % 4; // Generates a random number between 0 and 3
                        botInstance.bestStock = botInstance.stockNames[idx];
                        botInstance.bestPrice = botInstance.stockPrices[idx];
                        botInstance.day1 = false;
                    }
                    if(botInstance.currentStock == true)
                    {
                        botInstance.makeDecision(botInstance.ownedStock, botInstance.ownedStockPrice); // Make a decision
                    }
                    else{
                        botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice); // Make a decision

                    }
                    // Reset the counter based on the strategy
                    botInstance.counter = 1; // Make a decision every day

                }
                botInstance.stockCounter = 4;
            }
            botInstance.stockCounter--;
        }
    }


};

class Weekly : public BotStrategy {
public:
    Weekly() {}
    void onPriceUpdate(const QString &stockName, double newPrice)
    {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance

        if(botInstance.day1){
            botInstance.stockNames[botInstance.stockCounter] = stockName;
            botInstance.stockPrices[botInstance.stockCounter] = newPrice;}
        else{
            qDebug() << stockName << " | stockCounter: " << botInstance.stockCounter << " " << newPrice << " / " << botInstance.stockPrices[botInstance.stockCounter] << " = " << newPrice/botInstance.stockPrices[botInstance.stockCounter];
            if (botInstance.counter <= 1){
                if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange){
                    botInstance.bestChange = newPrice/botInstance.stockPrices[botInstance.stockCounter];
                    botInstance.bestStock = stockName;
                    botInstance.bestPrice = newPrice;
                }
            }
            botInstance.stockNames[botInstance.stockCounter] = stockName;
            botInstance.stockPrices[botInstance.stockCounter] = newPrice;
            if(stockName == botInstance.ownedStock)
            {
                botInstance.ownedStockPrice = newPrice;
            }
        }
        if(botInstance.stockCounter <= 0)
        {
            botInstance.counter--; // Decrement counter
            if (botInstance.counter <= 0) { // If counter is 0 or negative, make a decision
                if(botInstance.day1 == true)
                {
                    int idx = rand() % 4; // Generates a random number between 0 and 3
                    botInstance.bestStock = botInstance.stockNames[idx];
                    botInstance.bestPrice = botInstance.stockPrices[idx];
                    botInstance.day1 = false;
                }
                if(botInstance.currentStock == true)
                {
                    botInstance.makeDecision(botInstance.ownedStock, botInstance.ownedStockPrice); // Make a decision
                }
                else{
                    botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice); // Make a decision

                }
                botInstance.counter = 7; // Make a decision every 7 days

            }
            // For weekly strategy, output the number of days left until the bot is active
            if (botInstance.counter > 0) {
                qDebug() << botInstance.counter << "days left until bot is active on the Stock Market";
            }
            botInstance.stockCounter = 4;
        }
        botInstance.stockCounter--;
    }
private:


};

#endif // BOT_H
