#ifndef BOT_H
#define BOT_H

#include "stock.h"
#include <QDebug>
#include <QString>
#include <cstdlib>
#include "outputter.h"

class BotStrategy {
public:
    virtual void onPriceUpdate(const QString &stockName, double newPrice) = 0;
    virtual void makeDecision( QString stockNameThang, double currentPrice, QString stockNameThang2, double currentPrice2, QString stockNameThang3, double currentPrice3) = 0;

};

class Bot : public Observer {
public:
    static Bot &getInstance() { //this is SINGLETON
        static Bot instance;
        return instance;
    }

    void onPriceUpdate(const QString &stockName, double newPrice) override {
        if(strategy_ != nullptr){
           return strategy_->onPriceUpdate(stockName, newPrice);
        }
        return void();

    }

    void sellRemaining() {
        if(ownedStock != "") {
            qDebug() << "joe mama sold " << ownedStock << "at price " << ownedPriceUpdate;
            balance += ownedPriceUpdate;
            if(timeHeld1)
                logger_->logSell(ownedStock, ownedPriceUpdate - purchasePrice);
            else
                logger_->logSell(ownedStock, 0);
        }
        if(ownedStock2 != "") {
            qDebug() << "joe mama sold " << ownedStock2 << "at price " << ownedPriceUpdate2;
            balance += ownedPriceUpdate2;
            if(timeHeld2)
                logger_->logSell(ownedStock2, ownedPriceUpdate2 - purchasePrice2);
            else
                logger_->logSell(ownedStock2, 0);        }
        if(ownedStock3 != "") {
            qDebug() << "joe mama sold " << ownedStock3 << "at price " << ownedPriceUpdate3;
            balance += ownedPriceUpdate3;
            if(timeHeld3)
                logger_->logSell(ownedStock3, ownedPriceUpdate3 - purchasePrice3);
            else
                logger_->logSell(ownedStock3, 0);        }
    }

    void deposit(double num) {
        balance += num;
    }

    QString getBotBubble() {
        QString temp = "I currently own ";
        bool firstStock = false;

        if (!ownedStock.isEmpty() && timeHeld1 == 0) {
            temp += ownedStock + " at " + QString::number(purchasePrice);
            firstStock = true;
        }
        else if(!ownedStock.isEmpty())
        {
            temp += ownedStock + " at " + QString::number(ownedPriceUpdate);
            firstStock = true;
        }

        if (!ownedStock2.isEmpty() && timeHeld2 == 0) {
            if (firstStock) {
                temp += ",\nand ";
            }
            temp += ownedStock2 + " at " + QString::number(purchasePrice2);
            firstStock = true;
        } else if (!ownedStock2.isEmpty()) {
            if (firstStock) {
                temp += ",\nand ";
            }
            temp += ownedStock2 + " at " + QString::number(ownedPriceUpdate2);
            firstStock = true;
        }

        if (!ownedStock3.isEmpty() && timeHeld3 == 0) {
            if (firstStock) {
                temp += ",\nand ";
            }
            temp += ownedStock3 + " at " + QString::number(purchasePrice3);
        }
        else if (!ownedStock3.isEmpty()) {
            if (firstStock) {
                temp += ",\nand ";
            }
            temp += ownedStock3 + " at " + QString::number(ownedPriceUpdate3);
        }

        if (temp == "I currently own ") {
            temp = "Which one should I buy next??";
        } else {
            temp += "!!";
        }

        return temp;
    }



    double getBalance() const { return balance; }
    void setStrategy(BotStrategy *strategy) { strategy_ = strategy; }


    Bot() : balance(1000.0), ownedStock(""), ownedStock2(""), ownedStock3(""), purchasePrice(0.0), purchasePrice2(0.0), purchasePrice3(0.0), counter(1), stockCounter(7), strategy_(nullptr), logger_(nullptr){}

    // Prevent copying
    Bot(const Bot &) = delete;
    void operator=(const Bot &) = delete;

    void makeDecision( QString stockNameThang, double currentPrice, QString stockNameThang2, double currentPrice2, QString stockNameThang3, double currentPrice3) {
        if (strategy_ != nullptr) {
            strategy_->makeDecision(stockNameThang, currentPrice, stockNameThang2, currentPrice2, stockNameThang3, currentPrice3);  // Delegating the decision-making to the current strategy
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
    QString stockNames [8];
    double stockPrices[8];
    bool day1 = true;
    double bestDifference;
    QString bestStock;
    QString bestStock2;
    QString bestStock3;
    QString bestStock4;

    //variables to hold 3 best prices
    double bestPrice;
    double bestPrice2;
    double bestPrice3;
    double bestPrice4;
    bool currentStock = false;

    //variables to hold best change percentages
    double bestChange = 2;
    double bestChange2 = 2;
    double bestChange3 = 2;
    double bestChange4 = 2;
    //variables to compare prices
    double ownedStockPrice;
    double ownedStockPrice2;
    double ownedStockPrice3;

    double ownedPriceUpdate = 0.0;
    double ownedPriceUpdate2 = 0.0;
    double ownedPriceUpdate3 = 0.0;

    BotStrategy* strategy_;
    Outputter* logger_;

    int timeHeld1;
    int timeHeld2;
    int timeHeld3;
};

/*
 *
 * This strategy buys 3 stocks at once, not buying anymore stocks until all 3 have been sold.
 * Initially, 3 stocks are chosen at random. Then are sold at ANY profit or until they have been
 * held for 20 days.
 *
 * From here on out, 4 stocks are chosen based on the biggest 4 drops from the day prior, then the 4 stocks
 * are ranked by which ones have been sold for profit in the past. From here, the top 3 stocks are chosen for purchase.
 *
 * stocks are sold at ANY profit or when they have been held for 20 days.
 *
 */

class Daily : public BotStrategy {
public:
    Daily(){}

    void makeDecision( QString stockNameThang, double currentPrice, QString stockNameThang2, double currentPrice2, QString stockNameThang3, double currentPrice3) {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance

        if (botInstance.ownedStock.isEmpty() && botInstance.ownedStock2.isEmpty() && botInstance.ownedStock3.isEmpty()) {
            //buy stock if none is currently owned
            if (botInstance.balance >= currentPrice) {
                botInstance.balance -= currentPrice;
                botInstance.balance -= currentPrice2;
                botInstance.balance -= currentPrice3;
                botInstance.ownedStock = stockNameThang;
                botInstance.ownedStockPrice = currentPrice;

                botInstance.ownedStock2 = stockNameThang2;
                botInstance.ownedStockPrice2 = currentPrice2;

                botInstance.ownedStock3 = stockNameThang3;
                botInstance.ownedStockPrice3 = currentPrice3;

                botInstance.purchasePrice = currentPrice;
                botInstance.purchasePrice2 = currentPrice2;
                botInstance.purchasePrice3 = currentPrice3;

                botInstance.timeHeld1 = 0;
                botInstance.timeHeld2 = 0;
                botInstance.timeHeld3 = 0;

                qDebug() << "Bought 3 new shares of stock!";
                qDebug() << "-------------------------------";
                qDebug() << "#1 Ranked Stock: " << stockNameThang;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                qDebug() << "-------------------------------";
                qDebug() << "#2 Ranked Stock: " << stockNameThang2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                qDebug() << "-------------------------------";
                qDebug() << "#3 Ranked Stock: " << stockNameThang3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;


                botInstance.currentStock = true;
                botInstance.bestChange = 2;
                botInstance.bestChange2 = 2;
                botInstance.bestChange3 = 2;
                botInstance.bestChange4 = 2;
                botInstance.day1 = false;
            }
        } else  {
            // Check if we should sell first stock
            if ((currentPrice > botInstance.purchasePrice && !botInstance.ownedStock.isEmpty()) || botInstance.timeHeld1 >= 20) {
                botInstance.balance += currentPrice;
                qDebug() << "Sold 1 share of" << stockNameThang << "at $" << currentPrice << ". Held for " << botInstance.timeHeld1 << " days.";
                botInstance.timeHeld1 = 0;
                botInstance.bestChange = 2;
                botInstance.bestChange4 = 2;

                botInstance.ownedStock = ""; // No stock owned now
                //currentStock = false;
                botInstance.logger_->logSell(stockNameThang, currentPrice - botInstance.purchasePrice);
                botInstance.purchasePrice = 0.0;
            } else if(!botInstance.ownedStock.isEmpty()){
                qDebug() << "Holding" << stockNameThang << "at $" << currentPrice << ". Purchase price was $" << botInstance.purchasePrice;
                botInstance.timeHeld1++;
            }

            //checking second stock
            if (currentPrice2 > botInstance.purchasePrice2 && !botInstance.ownedStock2.isEmpty() || botInstance.timeHeld2 >= 20) {
                botInstance.balance += currentPrice2;
                qDebug() << "Sold 1 share of" << stockNameThang2 << "at $" << currentPrice2 << ". Held for " << botInstance.timeHeld2 << " days.";
                botInstance.timeHeld2 = 0;
                botInstance.bestChange2 = 2;
                botInstance.bestChange4 = 2;

                botInstance.ownedStock2 = ""; // No stock owned now
                //currentStock = false;
                botInstance.logger_->logSell(stockNameThang2, currentPrice2 - botInstance.purchasePrice2);
                botInstance.purchasePrice2 = 0.0;

            } else if(!botInstance.ownedStock2.isEmpty()){
                qDebug() << "Holding" << stockNameThang2 << "at $" << currentPrice2 << ". Purchase price was $" << botInstance.purchasePrice2;
                botInstance.timeHeld2++;
            }

            //checking third stock
            if (currentPrice3 > botInstance.purchasePrice3 && !botInstance.ownedStock3.isEmpty() || botInstance.timeHeld3 >= 20) {
                botInstance.balance += currentPrice3;
                qDebug() << "Sold 1 share of" << stockNameThang3 << "at $" << currentPrice3 << ". Held for " << botInstance.timeHeld3 << " days.";
                botInstance.timeHeld3 = 0;
                botInstance.bestChange3 = 2;
                botInstance.bestChange4 = 2;
                botInstance.ownedStock3 = ""; // No stock owned now
                //currentStock = false;
                botInstance.logger_->logSell(stockNameThang3, currentPrice3 - botInstance.purchasePrice3);
                botInstance.purchasePrice3 = 0.0;

            } else if(!botInstance.ownedStock3.isEmpty()){
                qDebug() << "Holding" << stockNameThang3 << "at $" << currentPrice3 << ". Purchase price was $" << botInstance.purchasePrice3;
                botInstance.timeHeld3++;
            }
            if (botInstance.ownedStock.isEmpty() && botInstance.ownedStock2.isEmpty() && botInstance.ownedStock3.isEmpty()) {
                botInstance.currentStock = false;
                botInstance.bestChange = 2;
                botInstance.bestChange2 = 2;
                botInstance.bestChange3 = 2;
                botInstance.bestChange4 = 2;



            }

        }
    }

    void onPriceUpdate(const QString &stockName, double newPrice)
    {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance

        {
            if(botInstance.day1){
                qDebug() << botInstance.day1;
                botInstance.stockNames[botInstance.stockCounter] = stockName;
                botInstance.stockPrices[botInstance.stockCounter] = newPrice;}
            else{
            qDebug() << stockName << " | stockCounter: " << botInstance.stockCounter << " " << newPrice << " / " << botInstance.stockPrices[botInstance.stockCounter] << " = " << newPrice/botInstance.stockPrices[botInstance.stockCounter];

                if(botInstance.ownedStock == stockName){
                    botInstance.ownedPriceUpdate = newPrice;
                }
                else if(botInstance.ownedStock2 == stockName){
                    botInstance.ownedPriceUpdate2 = newPrice;
                }
                else if(botInstance.ownedStock3 == stockName){
                    botInstance.ownedPriceUpdate3 = newPrice;
                }

                if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange){
                    botInstance.bestChange4 = botInstance.bestChange3;
                    botInstance.bestStock4 = botInstance.bestStock3;
                    botInstance.bestPrice4 = botInstance.bestPrice3;

                    botInstance.bestChange3 = botInstance.bestChange2; // moved to third best
                    botInstance.bestStock3 = botInstance.bestStock2;
                    botInstance.bestPrice3 = botInstance.bestPrice2;

                    botInstance.bestChange2 = botInstance.bestChange; // moved to second best
                    botInstance.bestStock2 = botInstance.bestStock;
                    botInstance.bestPrice2 = botInstance.bestPrice;

                    botInstance.bestChange = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated first best
                    botInstance.bestStock = stockName;
                    botInstance.bestPrice = newPrice;

                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange2)
                {
                    botInstance.bestChange4 = botInstance.bestChange3;
                    botInstance.bestStock4 = botInstance.bestStock3;
                    botInstance.bestPrice4 = botInstance.bestPrice3;

                    botInstance.bestChange3 = botInstance.bestChange2; // moved to third best
                    botInstance.bestStock3 = botInstance.bestStock2;
                    botInstance.bestPrice3 = botInstance.bestPrice2;

                    botInstance.bestChange2 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock2 = stockName;
                    botInstance.bestPrice2 = newPrice;

                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange3)
                {
                    botInstance.bestChange4 = botInstance.bestChange3;
                    botInstance.bestStock4 = botInstance.bestStock3;
                    botInstance.bestPrice4 = botInstance.bestPrice3;

                    botInstance.bestChange3 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock3 = stockName;
                    botInstance.bestPrice3 = newPrice;
                }
                else if(newPrice/botInstance.stockPrices[botInstance.stockCounter] < botInstance.bestChange4){
                    botInstance.bestChange4 = newPrice/botInstance.stockPrices[botInstance.stockCounter]; // updated second best
                    botInstance.bestStock4 = stockName;
                    botInstance.bestPrice4 = newPrice;
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
                        int idx = std::rand() % 8; // Generates a random number between 0 and 3
                        botInstance.bestStock = botInstance.stockNames[idx];
                        botInstance.bestPrice = botInstance.stockPrices[idx];
                        botInstance.bestStock2 = botInstance.stockNames[(idx + 1) %7];
                        botInstance.bestPrice2 = botInstance.stockPrices[(idx + 1) %7];

                        botInstance.bestStock3 = botInstance.stockNames[(idx + 2) %7];
                        botInstance.bestPrice3 = botInstance.stockPrices[(idx + 2) %7];
                        botInstance.day1 = false;
                        qDebug() << botInstance.bestStock << botInstance.bestStock2 << botInstance.bestStock3;

                    }
                    if(botInstance.currentStock == true)
                    {
                        botInstance.makeDecision(botInstance.ownedStock, botInstance.ownedStockPrice, botInstance.ownedStock2, botInstance.ownedStockPrice2, botInstance.ownedStock3, botInstance.ownedStockPrice3); // Make a decision
                    }
                    else{
                        // Compare the tally values and reorder bestStock, bestStock2, bestStock3

                        qDebug() << "Biggest Drops Today: #1 " << botInstance.bestStock << "; #2 " << botInstance.bestStock2 << "; #3 " << botInstance.bestStock3 << "; #4 " << botInstance.bestStock4;
                        // Compare bestStock, bestStock2, bestStock3, and bestStock4 and reorder them by their tally values

                        // Compare bestStock and bestStock2
                        if (botInstance.logger_->getTally(botInstance.bestStock) < botInstance.logger_->getTally(botInstance.bestStock2)) {
                            // Swap bestStock and bestStock2
                            QString tempStock = botInstance.bestStock;
                            double tempPrice = botInstance.bestPrice;
                            botInstance.bestStock = botInstance.bestStock2;
                            botInstance.bestPrice = botInstance.bestPrice2;
                            botInstance.bestStock2 = tempStock;
                            botInstance.bestPrice2 = tempPrice;
                        }

                        // Compare bestStock and bestStock3
                        if (botInstance.logger_->getTally(botInstance.bestStock) < botInstance.logger_->getTally(botInstance.bestStock3)) {
                            // Swap bestStock and bestStock3
                            QString tempStock = botInstance.bestStock;
                            double tempPrice = botInstance.bestPrice;
                            botInstance.bestStock = botInstance.bestStock3;
                            botInstance.bestPrice = botInstance.bestPrice3;
                            botInstance.bestStock3 = tempStock;
                            botInstance.bestPrice3 = tempPrice;
                        }

                        // Compare bestStock and bestStock4
                        if (botInstance.logger_->getTally(botInstance.bestStock) < botInstance.logger_->getTally(botInstance.bestStock4)) {
                            // Swap bestStock and bestStock4
                            QString tempStock = botInstance.bestStock;
                            double tempPrice = botInstance.bestPrice;
                            botInstance.bestStock = botInstance.bestStock4;
                            botInstance.bestPrice = botInstance.bestPrice4;
                            botInstance.bestStock4 = tempStock;
                            botInstance.bestPrice4 = tempPrice;
                        }

                        // Compare bestStock2 and bestStock3
                        if (botInstance.logger_->getTally(botInstance.bestStock2) < botInstance.logger_->getTally(botInstance.bestStock3)) {
                            // Swap bestStock2 and bestStock3
                            QString tempStock = botInstance.bestStock2;
                            double tempPrice = botInstance.bestPrice2;
                            botInstance.bestStock2 = botInstance.bestStock3;
                            botInstance.bestPrice2 = botInstance.bestPrice3;
                            botInstance.bestStock3 = tempStock;
                            botInstance.bestPrice3 = tempPrice;
                        }

                        // Compare bestStock2 and bestStock4
                        if (botInstance.logger_->getTally(botInstance.bestStock2) < botInstance.logger_->getTally(botInstance.bestStock4)) {
                            // Swap bestStock2 and bestStock4
                            QString tempStock = botInstance.bestStock2;
                            double tempPrice = botInstance.bestPrice2;
                            botInstance.bestStock2 = botInstance.bestStock4;
                            botInstance.bestPrice2 = botInstance.bestPrice4;
                            botInstance.bestStock4 = tempStock;
                            botInstance.bestPrice4 = tempPrice;
                        }

                        // Compare bestStock3 and bestStock4
                        if (botInstance.logger_->getTally(botInstance.bestStock3) < botInstance.logger_->getTally(botInstance.bestStock4)) {
                            // Swap bestStock3 and bestStock4
                            QString tempStock = botInstance.bestStock3;
                            double tempPrice = botInstance.bestPrice3;
                            botInstance.bestStock3 = botInstance.bestStock4;
                            botInstance.bestPrice3 = botInstance.bestPrice4;
                            botInstance.bestStock4 = tempStock;
                            botInstance.bestPrice4 = tempPrice;
                        }


                        qDebug() << "After taking into account the profit history: ";
                        qDebug() << "#1 " << botInstance.bestStock << " with tally count -> " << botInstance.logger_->getTally(botInstance.bestStock);
                        qDebug() << "#2 " << botInstance.bestStock2 << " with tally count -> " << botInstance.logger_->getTally(botInstance.bestStock2);
                        qDebug() << "#3 " << botInstance.bestStock3 << " with tally count -> " << botInstance.logger_->getTally(botInstance.bestStock3);

                        botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice, botInstance.bestStock2, botInstance.bestPrice2, botInstance.bestStock3, botInstance.bestPrice3); // Make a decision

                    }
                    // Reset the counter based on the strategy
                    botInstance.counter = 1; // Make a decision every day

                }
                botInstance.stockCounter = 8;
            }
            botInstance.stockCounter--;
        }
    }


};



/*
 * This strategy will buy a stock when it drops 2% from its day one price.
 *
 * The bot can own anywhere up to 3 stocks at one time.
 *
 * It will sell when it gets a profit of .5% and/or the stock has been held for 20 days
 *
 */

class lightMomentum : public BotStrategy {
public:
    lightMomentum(){}

    void makeDecision( QString stockNameThang, double currentPrice, QString stockNameThang2, double currentPrice2, QString stockNameThang3, double currentPrice3) {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance
        qDebug() << "new DAY YAYYYYYYY";
        botInstance.day1 = false;
        QString sold = "";
        QString sold2 = "";
        QString sold3 = "";

        //logic for first stock
        if(botInstance.ownedStock.isEmpty()){
            if(!stockNameThang.isEmpty() && stockNameThang!= botInstance.ownedStock2 && stockNameThang != botInstance.ownedStock3){
                botInstance.balance -= currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                botInstance.ownedStock = stockNameThang;
                botInstance.ownedStockPrice = currentPrice;
                botInstance.purchasePrice = currentPrice;
                stockNameThang = "";
                botInstance.timeHeld1 = 0;
            }
            else if(!stockNameThang2.isEmpty() && stockNameThang2!= botInstance.ownedStock2 && stockNameThang2 != botInstance.ownedStock3){
                botInstance.balance -= currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                botInstance.ownedStock = stockNameThang2;
                botInstance.ownedStockPrice = currentPrice2;
                botInstance.purchasePrice = currentPrice2;
                stockNameThang2 = "";
                botInstance.timeHeld1 = 0;
            }
            else if(!stockNameThang3.isEmpty() && stockNameThang3!= botInstance.ownedStock2 && stockNameThang3 != botInstance.ownedStock3){
                botInstance.balance -= currentPrice3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;
                botInstance.ownedStock = stockNameThang3;
                botInstance.ownedStockPrice = currentPrice3;
                botInstance.purchasePrice = currentPrice3;
                stockNameThang3 = "";
                botInstance.timeHeld1 = 0;
            }
        }
        else {
            if ((botInstance.ownedPriceUpdate > botInstance.purchasePrice * 1.005 && !botInstance.ownedStock.isEmpty()) || botInstance.timeHeld1 >= 20) {
            botInstance.balance += botInstance.ownedPriceUpdate;
            qDebug() << "Sold 1 share of" << botInstance.ownedStock << "at $" << botInstance.ownedPriceUpdate << ". Held for " << botInstance.timeHeld1 << " days.";
            botInstance.logger_->logSell(botInstance.ownedStock, botInstance.ownedPriceUpdate - botInstance.purchasePrice);
            botInstance.timeHeld1 = 0;
            sold = botInstance.ownedStock;
            botInstance.ownedStock = ""; // No stock owned now
            botInstance.purchasePrice = 0.0;
            } else if(!botInstance.ownedStock.isEmpty()){
            qDebug() << "Holding" << botInstance.ownedStock << "at $" << botInstance.ownedPriceUpdate << ". Purchase price was $" << botInstance.purchasePrice << "; Current days held: " << botInstance.timeHeld1;
            botInstance.timeHeld1++;
            }
        }


        //logic for second stock
        if(botInstance.ownedStock2.isEmpty()){

            if(!stockNameThang.isEmpty() && stockNameThang != botInstance.ownedStock && stockNameThang != botInstance.ownedStock3 && stockNameThang != sold){
                botInstance.balance -= currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                botInstance.ownedStock2 = stockNameThang;
                botInstance.ownedStockPrice2 = currentPrice;
                botInstance.purchasePrice2 = currentPrice;
                stockNameThang = "";
                botInstance.timeHeld2 = 0;

            }
            else if(!stockNameThang2.isEmpty() && stockNameThang2 != botInstance.ownedStock && stockNameThang2 != botInstance.ownedStock3 && stockNameThang2 != sold){
                botInstance.balance -= currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                botInstance.ownedStock2 = stockNameThang2;
                botInstance.ownedStockPrice2 = currentPrice2;
                botInstance.purchasePrice2 = currentPrice2;
                stockNameThang2 = "";
                botInstance.timeHeld2 = 0;

            }
            else if(!stockNameThang3.isEmpty() && stockNameThang3 != botInstance.ownedStock && stockNameThang3 != botInstance.ownedStock3 && stockNameThang3 != sold){
                botInstance.balance -= currentPrice3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;
                botInstance.ownedStock2 = stockNameThang3;
                botInstance.ownedStockPrice2 = currentPrice3;
                botInstance.purchasePrice2 = currentPrice3;
                stockNameThang3 = "";
                botInstance.timeHeld2 = 0;
            }

        }
        else {
            if ((botInstance.ownedPriceUpdate2 > botInstance.purchasePrice2 * 1.005 && !botInstance.ownedStock2.isEmpty()) || botInstance.timeHeld2 >= 20) {
                botInstance.balance += botInstance.ownedPriceUpdate2;
                qDebug() << "Sold 1 share of" << botInstance.ownedStock2 << "at $" << botInstance.ownedPriceUpdate2 << ". Held for " << botInstance.timeHeld2 << " days.";
                botInstance.logger_->logSell(botInstance.ownedStock2, botInstance.ownedPriceUpdate2 - botInstance.purchasePrice2);
                botInstance.timeHeld2 = 0;
                sold2 = botInstance.ownedStock2;
                botInstance.ownedStock2 = ""; // No stock owned now
                botInstance.purchasePrice2 = 0.0;
            } else if(!botInstance.ownedStock2.isEmpty()) {
                qDebug() << "Holding" << botInstance.ownedStock2 << "at $" << botInstance.ownedPriceUpdate2 << ". Purchase price was $" << botInstance.purchasePrice2 << "; Current days held: " << botInstance.timeHeld2;
                botInstance.timeHeld2++;
            }
        }

        //logic for third stock
        if(botInstance.ownedStock3.isEmpty()){
            if(!stockNameThang.isEmpty() && stockNameThang != botInstance.ownedStock && stockNameThang != botInstance.ownedStock2 && stockNameThang != sold && stockNameThang != sold2){
                botInstance.balance -= currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                botInstance.ownedStock3 = stockNameThang;
                botInstance.ownedStockPrice3 = currentPrice;
                botInstance.purchasePrice3 = currentPrice;
                stockNameThang = "";
                botInstance.timeHeld3 = 0;

            }
            else if(!stockNameThang2.isEmpty() && stockNameThang2 != botInstance.ownedStock && stockNameThang2 != botInstance.ownedStock2 && stockNameThang2 != sold && stockNameThang2 != sold2){
                botInstance.balance -= currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                botInstance.ownedStock3 = stockNameThang2;
                botInstance.ownedStockPrice3 = currentPrice2;
                botInstance.purchasePrice3 = currentPrice2;
                stockNameThang2 = "";
                botInstance.timeHeld3 = 0;

            }
            else if(!stockNameThang3.isEmpty() && stockNameThang3 != botInstance.ownedStock && stockNameThang3 != botInstance.ownedStock2 && stockNameThang3 != sold && stockNameThang3 != sold2){
                botInstance.balance -= currentPrice3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;
                botInstance.ownedStock3 = stockNameThang3;
                botInstance.ownedStockPrice3 = currentPrice3;
                botInstance.purchasePrice3 = currentPrice3;
                stockNameThang3 = "";
                botInstance.timeHeld3 = 0;
            }

        }
        else {
            if ((botInstance.ownedPriceUpdate3 > botInstance.purchasePrice3 * 1.005 && !botInstance.ownedStock3.isEmpty()) || botInstance.timeHeld3 >= 20) {
                botInstance.balance += botInstance.ownedPriceUpdate3;
                qDebug() << "Sold 1 share of" << botInstance.ownedStock3 << "at $" << botInstance.ownedPriceUpdate3 << ". Held for " << botInstance.timeHeld3 << " days.";
                botInstance.logger_->logSell(botInstance.ownedStock3, botInstance.ownedPriceUpdate3 - botInstance.purchasePrice3);
                botInstance.timeHeld3 = 0;
                botInstance.ownedStock3 = ""; // No stock owned now
                botInstance.purchasePrice = 0.0;
            } else if(!botInstance.ownedStock3.isEmpty()){
                qDebug() << "Holding" << botInstance.ownedStock3 << "at $" << botInstance.ownedPriceUpdate3 << ". Purchase price was $" << botInstance.purchasePrice3 << "; Current days held: " << botInstance.timeHeld3;
                botInstance.timeHeld3++;
            }
        }

        botInstance.bestStock = "";
        botInstance.bestStock2 = "";
        botInstance.bestStock3 = "";

        botInstance.bestPrice = 0.0;
        botInstance.bestPrice2 = 0.0;
        botInstance.bestPrice3 = 0.0;


    }


    void onPriceUpdate(const QString &stockName, double newPrice)
    {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance
        double change1 = 0.0;
        double change2 = 0.0;
        double change3 = 0.0;

            if(botInstance.day1){
                botInstance.stockNames[botInstance.stockCounter] = stockName;
                botInstance.stockPrices[botInstance.stockCounter] = newPrice;

            }
            else{
                double currentChange = newPrice / botInstance.stockPrices[botInstance.stockCounter];

                if(botInstance.ownedStock == stockName){
                    botInstance.ownedPriceUpdate = newPrice;
                }
                else if(botInstance.ownedStock2 == stockName){
                    botInstance.ownedPriceUpdate2 = newPrice;
                }
                else if(botInstance.ownedStock3 == stockName){
                    botInstance.ownedPriceUpdate3 = newPrice;
                }

                if(currentChange <=.98){
                    if(botInstance.bestStock.isEmpty())
                    {
                        change1 = currentChange;
                        botInstance.bestStock = stockName;
                        botInstance.bestPrice = newPrice;
                    }
                    else if(botInstance.bestStock2.isEmpty())
                    {
                        change2 = currentChange;
                        botInstance.bestStock2 = stockName;
                        botInstance.bestPrice2 = newPrice;
                    }
                    else if(botInstance.bestStock3.isEmpty())
                    {
                        change3 = currentChange;
                        botInstance.bestStock3 = stockName;
                        botInstance.bestPrice3 = newPrice;
                    }
                    else {

                        if (change1 > change2) {
                            std::swap(change1, change2);
                            std::swap(botInstance.bestStock, botInstance.bestStock2);
                            std::swap(botInstance.bestPrice, botInstance.bestPrice2);
                        }

                        if (change2 > change3) {
                            std::swap(change2, change3);
                            std::swap(botInstance.bestStock2, botInstance.bestStock3);
                            std::swap(botInstance.bestPrice2, botInstance.bestPrice3);
                        }

                        if (change1 > change2) {
                            std::swap(change1, change2);
                            std::swap(botInstance.bestStock, botInstance.bestStock2);
                            std::swap(botInstance.bestPrice, botInstance.bestPrice2);
                        }

                        // All three bestStock, bestStock2, and bestStock3 are filled, so compare to see where the new stock should go
                        if (currentChange <= change1) {
                            // Shift down: change1 -> change2, change2 -> change3
                            change3 = change2;
                            botInstance.bestStock3 = botInstance.bestStock2;
                            botInstance.bestPrice3 = botInstance.bestPrice2;

                            change2 = change1;
                            botInstance.bestStock2 = botInstance.bestStock;
                            botInstance.bestPrice2 = botInstance.bestPrice;

                            // Now put the current stock in the first position
                            change1 = currentChange;
                            botInstance.bestStock = stockName;
                            botInstance.bestPrice = newPrice;
                        } else if (currentChange <= change2) {
                            // Shift down: change2 -> change3
                            change3 = change2;
                            botInstance.bestStock3 = botInstance.bestStock2;
                            botInstance.bestPrice3 = botInstance.bestPrice2;

                            // Now put the current stock in the second position
                            change2 = currentChange;
                            botInstance.bestStock2 = stockName;
                            botInstance.bestPrice2 = newPrice;
                        } else if (currentChange <= change3) {
                            // Now put the current stock in the third position
                            change3 = currentChange;
                            botInstance.bestStock3 = stockName;
                            botInstance.bestPrice3 = newPrice;
                        }
                    }
            }


        }
            if(botInstance.stockCounter <= 0)
            {
                botInstance.counter--; // Decrement counter
                if (botInstance.counter <= 0) { // If counter is 0 or negative, make a decision
                    if(botInstance.day1 == true)
                    {
                        botInstance.day1 = false;

                    }

                    botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice, botInstance.bestStock2, botInstance.bestPrice2, botInstance.bestStock3, botInstance.bestPrice3); // Make a decision


                    // Reset the counter based on the strategy
                    botInstance.counter = 1; // Make a decision every day

                }
                botInstance.stockCounter = 8;
            }
            botInstance.stockCounter--;



}

};


/*
 * This strategy buys when the price of a stock drops 3% from its day one price.
 *
 * The bot can own anywhere up to 3 stocks at one time.
 *
 * It will only sell the stock when it makes a 1% profit and/or it has been held for 20 days.
 *
 */

class heavyMomentum : public BotStrategy {
public:
    heavyMomentum(){}

    void makeDecision( QString stockNameThang, double currentPrice, QString stockNameThang2, double currentPrice2, QString stockNameThang3, double currentPrice3) {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance
        qDebug() << "new DAY YAYYYYYYY";
        botInstance.day1 = false;
        QString sold = "";
        QString sold2 = "";
        QString sold3 = "";

        //logic for first stock
        if(botInstance.ownedStock.isEmpty()){
            if(!stockNameThang.isEmpty() && stockNameThang!= botInstance.ownedStock2 && stockNameThang != botInstance.ownedStock3){
                botInstance.balance -= currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                botInstance.ownedStock = stockNameThang;
                botInstance.ownedStockPrice = currentPrice;
                botInstance.purchasePrice = currentPrice;
                stockNameThang = "";
                botInstance.timeHeld1 = 0;
            }
            else if(!stockNameThang2.isEmpty() && stockNameThang2!= botInstance.ownedStock2 && stockNameThang2 != botInstance.ownedStock3){
                botInstance.balance -= currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                botInstance.ownedStock = stockNameThang2;
                botInstance.ownedStockPrice = currentPrice2;
                botInstance.purchasePrice = currentPrice2;
                stockNameThang2 = "";
                botInstance.timeHeld1 = 0;
            }
            else if(!stockNameThang3.isEmpty() && stockNameThang3!= botInstance.ownedStock2 && stockNameThang3 != botInstance.ownedStock3){
                botInstance.balance -= currentPrice3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;
                botInstance.ownedStock = stockNameThang3;
                botInstance.ownedStockPrice = currentPrice3;
                botInstance.purchasePrice = currentPrice3;
                stockNameThang3 = "";
                botInstance.timeHeld1 = 0;
            }
        }
        else {
            if ((botInstance.ownedPriceUpdate > botInstance.purchasePrice * 1.01 && !botInstance.ownedStock.isEmpty()) || botInstance.timeHeld1 >= 20) {
                botInstance.balance += botInstance.ownedPriceUpdate;
                qDebug() << "Sold 1 share of" << botInstance.ownedStock << "at $" << botInstance.ownedPriceUpdate << ". Held for " << botInstance.timeHeld1 << " days.";
                botInstance.logger_->logSell(botInstance.ownedStock, botInstance.ownedPriceUpdate - botInstance.purchasePrice);
                botInstance.timeHeld1 = 0;
                sold = botInstance.ownedStock;
                botInstance.ownedStock = ""; // No stock owned now
                botInstance.purchasePrice = 0.0;
            } else if(!botInstance.ownedStock.isEmpty()){
                qDebug() << "Holding" << botInstance.ownedStock << "at $" << botInstance.ownedPriceUpdate << ". Purchase price was $" << botInstance.purchasePrice << "; Current days held: " << botInstance.timeHeld1;
                botInstance.timeHeld1++;
            }
        }


        //logic for second stock
        if(botInstance.ownedStock2.isEmpty()){

            if(!stockNameThang.isEmpty() && stockNameThang != botInstance.ownedStock && stockNameThang != botInstance.ownedStock3 && stockNameThang != sold){
                botInstance.balance -= currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                botInstance.ownedStock2 = stockNameThang;
                botInstance.ownedStockPrice2 = currentPrice;
                botInstance.purchasePrice2 = currentPrice;
                stockNameThang = "";
                botInstance.timeHeld2 = 0;

            }
            else if(!stockNameThang2.isEmpty() && stockNameThang2 != botInstance.ownedStock && stockNameThang2 != botInstance.ownedStock3 && stockNameThang2 != sold){
                botInstance.balance -= currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                botInstance.ownedStock2 = stockNameThang2;
                botInstance.ownedStockPrice2 = currentPrice2;
                botInstance.purchasePrice2 = currentPrice2;
                stockNameThang2 = "";
                botInstance.timeHeld2 = 0;

            }
            else if(!stockNameThang3.isEmpty() && stockNameThang3 != botInstance.ownedStock && stockNameThang3 != botInstance.ownedStock3 && stockNameThang3 != sold){
                botInstance.balance -= currentPrice3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;
                botInstance.ownedStock2 = stockNameThang3;
                botInstance.ownedStockPrice2 = currentPrice3;
                botInstance.purchasePrice2 = currentPrice3;
                stockNameThang3 = "";
                botInstance.timeHeld2 = 0;
            }

        }
        else {
            if ((botInstance.ownedPriceUpdate2 > botInstance.purchasePrice2 * 1.01 && !botInstance.ownedStock2.isEmpty()) || botInstance.timeHeld2 >= 20) {
                botInstance.balance += botInstance.ownedPriceUpdate2;
                qDebug() << "Sold 1 share of" << botInstance.ownedStock2 << "at $" << botInstance.ownedPriceUpdate2 << ". Held for " << botInstance.timeHeld2 << " days.";
                botInstance.logger_->logSell(botInstance.ownedStock2, botInstance.ownedPriceUpdate2 - botInstance.purchasePrice2);
                botInstance.timeHeld2 = 0;
                sold2 = botInstance.ownedStock2;
                botInstance.ownedStock2 = ""; // No stock owned now
                botInstance.purchasePrice2 = 0.0;
            } else if(!botInstance.ownedStock2.isEmpty()) {
                qDebug() << "Holding" << botInstance.ownedStock2 << "at $" << botInstance.ownedPriceUpdate2 << ". Purchase price was $" << botInstance.purchasePrice2 << "; Current days held: " << botInstance.timeHeld2;
                botInstance.timeHeld2++;
            }
        }

        //logic for third stock
        if(botInstance.ownedStock3.isEmpty()){
            if(!stockNameThang.isEmpty() && stockNameThang != botInstance.ownedStock && stockNameThang != botInstance.ownedStock2 && stockNameThang != sold && stockNameThang != sold2){
                botInstance.balance -= currentPrice;
                qDebug() << "Purchased 1 share of " << stockNameThang << "at $" << currentPrice;
                botInstance.ownedStock3 = stockNameThang;
                botInstance.ownedStockPrice3 = currentPrice;
                botInstance.purchasePrice3 = currentPrice;
                stockNameThang = "";
                botInstance.timeHeld3 = 0;

            }
            else if(!stockNameThang2.isEmpty() && stockNameThang2 != botInstance.ownedStock && stockNameThang2 != botInstance.ownedStock2 && stockNameThang2 != sold && stockNameThang2 != sold2){
                botInstance.balance -= currentPrice2;
                qDebug() << "Purchased 1 share of " << stockNameThang2 << "at $" << currentPrice2;
                botInstance.ownedStock3 = stockNameThang2;
                botInstance.ownedStockPrice3 = currentPrice2;
                botInstance.purchasePrice3 = currentPrice2;
                stockNameThang2 = "";
                botInstance.timeHeld3 = 0;

            }
            else if(!stockNameThang3.isEmpty() && stockNameThang3 != botInstance.ownedStock && stockNameThang3 != botInstance.ownedStock2 && stockNameThang3 != sold && stockNameThang3 != sold2){
                botInstance.balance -= currentPrice3;
                qDebug() << "Purchased 1 share of " << stockNameThang3 << "at $" << currentPrice3;
                botInstance.ownedStock3 = stockNameThang3;
                botInstance.ownedStockPrice3 = currentPrice3;
                botInstance.purchasePrice3 = currentPrice3;
                stockNameThang3 = "";
                botInstance.timeHeld3 = 0;
            }

        }
        else {
            if ((botInstance.ownedPriceUpdate3 > botInstance.purchasePrice3 * 1.01 && !botInstance.ownedStock3.isEmpty()) || botInstance.timeHeld3 >= 20) {
                botInstance.balance += botInstance.ownedPriceUpdate3;
                qDebug() << "Sold 1 share of" << botInstance.ownedStock3 << "at $" << botInstance.ownedPriceUpdate3 << ". Held for " << botInstance.timeHeld3 << " days.";
                botInstance.logger_->logSell(botInstance.ownedStock3, botInstance.ownedPriceUpdate3 - botInstance.purchasePrice3);
                botInstance.timeHeld3 = 0;
                botInstance.ownedStock3 = ""; // No stock owned now
                botInstance.purchasePrice = 0.0;
            } else if(!botInstance.ownedStock3.isEmpty()){
                qDebug() << "Holding" << botInstance.ownedStock3 << "at $" << botInstance.ownedPriceUpdate3 << ". Purchase price was $" << botInstance.purchasePrice3 << "; Current days held: " << botInstance.timeHeld3;
                botInstance.timeHeld3++;
            }
        }

        botInstance.bestStock = "";
        botInstance.bestStock2 = "";
        botInstance.bestStock3 = "";

        botInstance.bestPrice = 0.0;
        botInstance.bestPrice2 = 0.0;
        botInstance.bestPrice3 = 0.0;


    }


    void onPriceUpdate(const QString &stockName, double newPrice)
    {
        Bot &botInstance = Bot::getInstance();  // Always get the singleton instance
        double change1 = 0.0;
        double change2 = 0.0;
        double change3 = 0.0;

        if(botInstance.day1){
            botInstance.stockNames[botInstance.stockCounter] = stockName;
            botInstance.stockPrices[botInstance.stockCounter] = newPrice;

        }
        else{
            double currentChange = newPrice / botInstance.stockPrices[botInstance.stockCounter];

            if(botInstance.ownedStock == stockName){
                botInstance.ownedPriceUpdate = newPrice;
            }
            else if(botInstance.ownedStock2 == stockName){
                botInstance.ownedPriceUpdate2 = newPrice;
            }
            else if(botInstance.ownedStock3 == stockName){
                botInstance.ownedPriceUpdate3 = newPrice;
            }

            if(currentChange <=.97){
                if(botInstance.bestStock.isEmpty())
                {
                    change1 = currentChange;
                    botInstance.bestStock = stockName;
                    botInstance.bestPrice = newPrice;
                }
                else if(botInstance.bestStock2.isEmpty())
                {
                    change2 = currentChange;
                    botInstance.bestStock2 = stockName;
                    botInstance.bestPrice2 = newPrice;
                }
                else if(botInstance.bestStock3.isEmpty())
                {
                    change3 = currentChange;
                    botInstance.bestStock3 = stockName;
                    botInstance.bestPrice3 = newPrice;
                }
                else {

                    if (change1 > change2) {
                        std::swap(change1, change2);
                        std::swap(botInstance.bestStock, botInstance.bestStock2);
                        std::swap(botInstance.bestPrice, botInstance.bestPrice2);
                    }

                    if (change2 > change3) {
                        std::swap(change2, change3);
                        std::swap(botInstance.bestStock2, botInstance.bestStock3);
                        std::swap(botInstance.bestPrice2, botInstance.bestPrice3);
                    }

                    if (change1 > change2) {
                        std::swap(change1, change2);
                        std::swap(botInstance.bestStock, botInstance.bestStock2);
                        std::swap(botInstance.bestPrice, botInstance.bestPrice2);
                    }

                    // All three bestStock, bestStock2, and bestStock3 are filled, so compare to see where the new stock should go
                    if (currentChange <= change1) {
                        // Shift down: change1 -> change2, change2 -> change3
                        change3 = change2;
                        botInstance.bestStock3 = botInstance.bestStock2;
                        botInstance.bestPrice3 = botInstance.bestPrice2;

                        change2 = change1;
                        botInstance.bestStock2 = botInstance.bestStock;
                        botInstance.bestPrice2 = botInstance.bestPrice;

                        // Now put the current stock in the first position
                        change1 = currentChange;
                        botInstance.bestStock = stockName;
                        botInstance.bestPrice = newPrice;
                    } else if (currentChange <= change2) {
                        // Shift down: change2 -> change3
                        change3 = change2;
                        botInstance.bestStock3 = botInstance.bestStock2;
                        botInstance.bestPrice3 = botInstance.bestPrice2;

                        // Now put the current stock in the second position
                        change2 = currentChange;
                        botInstance.bestStock2 = stockName;
                        botInstance.bestPrice2 = newPrice;
                    } else if (currentChange <= change3) {
                        // Now put the current stock in the third position
                        change3 = currentChange;
                        botInstance.bestStock3 = stockName;
                        botInstance.bestPrice3 = newPrice;
                    }
                }
            }


        }
        if(botInstance.stockCounter <= 0)
        {
            botInstance.counter--; // Decrement counter
            if (botInstance.counter <= 0) { // If counter is 0 or negative, make a decision
                if(botInstance.day1 == true)
                {
                    botInstance.day1 = false;

                }

                botInstance.makeDecision(botInstance.bestStock, botInstance.bestPrice, botInstance.bestStock2, botInstance.bestPrice2, botInstance.bestStock3, botInstance.bestPrice3); // Make a decision


                // Reset the counter based on the strategy
                botInstance.counter = 1; // Make a decision every day
            }
            botInstance.stockCounter = 8;
        }
        botInstance.stockCounter--;



    }

};


#endif
