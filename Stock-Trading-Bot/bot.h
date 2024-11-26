#ifndef BOT_H
#define BOT_H

#include "stock.h"
#include <QDebug>
#include <QString>
#include <cstdlib> // For rand()
#include "outputter.h"

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
        QString temp = "I currently own ";
        bool firstStock = false;

        if (!ownedStock.isEmpty()) {
            temp += ownedStock + " at " + QString::number(ownedStockPrice);
            firstStock = true;
        }

        if (!ownedStock2.isEmpty()) {
            if (firstStock) {
                temp += ",\nand ";
            }
            temp += ownedStock2 + " at " + QString::number(ownedStockPrice2);
            firstStock = true;
        }

        if (!ownedStock3.isEmpty()) {
            if (firstStock) {
                temp += ",\nand ";
            }
            temp += ownedStock3 + " at " + QString::number(ownedStockPrice3);
        }

        if (temp == "I currently own ") {
            temp = "Which one should I buy next??";
        } else {
            temp += "!!";
        }

        return temp;
    }



    double getBalance() const { return balance; }
    void setStrategy(BotStrategy *strategy) { strategy_ = strategy; } // No more error


    Bot() : balance(1000.0), ownedStock(""), ownedStock2(""), ownedStock3(""), purchasePrice(0.0), purchasePrice2(0.0), purchasePrice3(0.0), counter(1), stockCounter(3), strategy_(nullptr), logger_(nullptr){}

    // Prevent copying
    Bot(const Bot &) = delete;
    void operator=(const Bot &) = delete;

    void makeDecision( QString stockNameThang, double currentPrice, QString stockNameThang2, double currentPrice2, QString stockNameThang3, double currentPrice3) {
        if (ownedStock.isEmpty() && ownedStock2.isEmpty() && ownedStock3.isEmpty()) {
            // Buy stock if none is currently owned
            if (balance >= currentPrice) {
                balance -= currentPrice;
                balance -= currentPrice2;
                balance -= currentPrice3;
                ownedStock = stockNameThang;
                ownedStockPrice = currentPrice;

                ownedStock2 = stockNameThang2;
                ownedStockPrice2 = currentPrice2;

                ownedStock3 = stockNameThang3;
                ownedStockPrice3 = currentPrice3;

                purchasePrice = currentPrice;
                purchasePrice2 = currentPrice2;
                purchasePrice3 = currentPrice3;

                qDebug() << "Bought 3 new shares of stock!" << "---------------";
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;


                currentStock = true;
                bestChange = 2;
                bestChange2 = 2;
                bestChange3 = 2;
                day1 = false;
            }
        } else  {
            // Check if we should sell first stock
            if (currentPrice > purchasePrice && !ownedStock.isEmpty()) {
                balance += currentPrice;
                qDebug() << "Sold 1 share of" << stockNameThang << "at $" << currentPrice;
                bestChange = 2;
                ownedStock = ""; // No stock owned now
                //currentStock = false;
                purchasePrice = 0.0;
            } else if(!ownedStock.isEmpty()){
                qDebug() << "Holding" << stockNameThang << "at $" << currentPrice << ". Purchase price was $" << purchasePrice;
            }

            //checking second stock
            if (currentPrice2 > purchasePrice2 && !ownedStock2.isEmpty()) {
                balance += currentPrice2;
                qDebug() << "Sold 1 share of" << stockNameThang2 << "at $" << currentPrice2;
                bestChange2 = 2;
                ownedStock2 = ""; // No stock owned now
                //currentStock = false;
                purchasePrice2 = 0.0;
            } else if(!ownedStock2.isEmpty()){
                qDebug() << "Holding" << stockNameThang2 << "at $" << currentPrice2 << ". Purchase price was $" << purchasePrice2;
            }

            //checking third stock
            if (currentPrice3 > purchasePrice3 && !ownedStock3.isEmpty()) {
                balance += currentPrice3;
                qDebug() << "Sold 1 share of" << stockNameThang3 << "at $" << currentPrice3;
                bestChange3 = 2;
                ownedStock3 = ""; // No stock owned now
                //currentStock = false;
                purchasePrice3 = 0.0;
            } else if(!ownedStock3.isEmpty()){
                qDebug() << "Holding" << stockNameThang3 << "at $" << currentPrice3 << ". Purchase price was $" << purchasePrice3;
            }
            if (ownedStock.isEmpty() && ownedStock2.isEmpty() && ownedStock3.isEmpty()) {
                currentStock = false;
                bestChange = 2;
                bestChange2 = 2;
                bestChange3 = 2;



            }

        }
    }

    void attatchLogger(Outputter *thang) {
        logger_ = thang;
    }

    double balance;

    //variables to hold currently owned stocks
    QString ownedStock;
    QString ownedStock2;
    QString ownedStock3;

    double purchasePrice;
    double purchasePrice2;
    double purchasePrice3;

    int counter;
    int stockCounter;
    QString stockNames [4];
    double stockPrices[4];
    bool day1 = true;
    double bestDifference;
    QString bestStock;
    QString bestStock2;
    QString bestStock3;

    //variables to hold 3 best prices
    double bestPrice;
    double bestPrice2;
    double bestPrice3;
    bool currentStock = false;

    //variables to hold best change percentages
    double bestChange = 2;
    double bestChange2 = 2;
    double bestChange3 = 2;

    //variables to compare prices
    double ownedStockPrice;
    double ownedStockPrice2;
    double ownedStockPrice3;
    BotStrategy* strategy_;
    Outputter* logger_;
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
                    botInstance.bestChange2 = botInstance.bestChange; // moved to second best
                    botInstance.bestStock2 = botInstance.bestStock;
                    botInstance.bestPrice2 = botInstance.bestPrice;

                    botInstance.bestChange = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated first best
                    botInstance.bestStock = stockName;
                    botInstance.bestPrice = newPrice;

                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange2)
                {
                    botInstance.bestChange3 = botInstance.bestChange2; // moved to third best
                    botInstance.bestStock3 = botInstance.bestStock2;
                    botInstance.bestPrice3 = botInstance.bestPrice2;

                    botInstance.bestChange2 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock2 = stockName;
                    botInstance.bestPrice2 = newPrice;

                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange3)
                {
                    botInstance.bestChange3 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock3 = stockName;
                    botInstance.bestPrice3 = newPrice;
                }

                botInstance.stockNames[botInstance.stockCounter] = stockName;
                botInstance.stockPrices[botInstance.stockCounter] = newPrice;
                if(stockName == botInstance.ownedStock)
                {
                    botInstance.ownedStockPrice = newPrice;
                }
                else if(stockName == botInstance.ownedStock2)
                {
                    botInstance.ownedStockPrice2 = newPrice;
                }
                else if(stockName == botInstance.ownedStock3)
                {
                    botInstance.ownedStockPrice3 = newPrice;
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

                        botInstance.bestStock2 = botInstance.stockNames[(idx + 1) %3];
                        botInstance.bestPrice2 = botInstance.stockPrices[(idx + 1) %3];

                        botInstance.bestStock3 = botInstance.stockNames[(idx + 2) %3];
                        botInstance.bestPrice3 = botInstance.stockPrices[(idx + 2) %3];
                        botInstance.day1 = false;
                    }
                    if(botInstance.currentStock == true)
                    {
                        botInstance.makeDecision(botInstance.ownedStock, botInstance.ownedStockPrice, botInstance.ownedStock2, botInstance.ownedStockPrice2, botInstance.ownedStock3, botInstance.ownedStockPrice3); // Make a decision
                    }
                    else{
                        botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice, botInstance.bestStock2, botInstance.bestPrice2, botInstance.bestStock3, botInstance.bestPrice3); // Make a decision

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
                    botInstance.bestChange2 = botInstance.bestChange; // moved to second best
                    botInstance.bestStock2 = botInstance.bestStock;
                    botInstance.bestPrice2 = botInstance.bestPrice;

                    botInstance.bestChange = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated first best
                    botInstance.bestStock = stockName;
                    botInstance.bestPrice = newPrice;

                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange2)
                {
                    botInstance.bestChange3 = botInstance.bestChange2; // moved to third best
                    botInstance.bestStock3 = botInstance.bestStock2;
                    botInstance.bestPrice3 = botInstance.bestPrice2;

                    botInstance.bestChange2 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock2 = stockName;
                    botInstance.bestPrice2 = newPrice;

                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange3)
                {
                    botInstance.bestChange3 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock3 = stockName;
                    botInstance.bestPrice3 = newPrice;
                }
            }
            botInstance.stockNames[botInstance.stockCounter] = stockName;
            botInstance.stockPrices[botInstance.stockCounter] = newPrice;
            if(stockName == botInstance.ownedStock)
            {
                botInstance.ownedStockPrice = newPrice;
            }
            else if(stockName == botInstance.ownedStock2)
            {
                botInstance.ownedStockPrice2 = newPrice;
            }
            else if(stockName == botInstance.ownedStock3)
            {
                botInstance.ownedStockPrice3 = newPrice;
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

                    botInstance.bestStock2 = botInstance.stockNames[(idx + 1) %3];
                    botInstance.bestPrice2 = botInstance.stockPrices[(idx + 1) %3];

                    botInstance.bestStock3 = botInstance.stockNames[(idx + 2) %3];
                    botInstance.bestPrice3 = botInstance.stockPrices[(idx + 2) %3];

                    botInstance.day1 = false;
                }
                if(botInstance.currentStock == true)
                {
                    botInstance.makeDecision(botInstance.ownedStock, botInstance.ownedStockPrice, botInstance.ownedStock2, botInstance.ownedStockPrice2, botInstance.ownedStock3, botInstance.ownedStockPrice3); // Make a decision
                }
                else{
                    botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice, botInstance.bestStock2, botInstance.bestPrice2, botInstance.bestStock3, botInstance.bestPrice3); // Make a decision

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
