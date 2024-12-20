#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include "stockfactory.h"
#include "stock.h"
#include "bot.h"
#include <QObject>
#include <vector>
#include "outputter.h"


/*
 * This is facade implementation
 *
 */
class SimulationManager : public QObject {
    Q_OBJECT

public:
    SimulationManager(QObject *parent = nullptr) : QObject(parent) {}
    const std::vector<std::shared_ptr<Stock>>& getStocks() const { return stocks; }

    void startSimulation(Outputter &logger, QString lel) {
        BotStrategy *strategy = nullptr;

        if(lel == "dailyStrategy")
            strategy = new Daily();
        else if(lel == "lightMomentumStrategy")
            strategy = new lightMomentum();
        else if(lel == "hmomentumStrategy")
            strategy = new heavyMomentum();

        std::shared_ptr<Bot> bot = std::shared_ptr<Bot>(&Bot::getInstance(), [](Bot*) {});
        Bot::getInstance().setStrategy(strategy);
        Bot::getInstance().attatchLogger(&logger);
        logger_ = &logger;

       std::shared_ptr<Stock> REGAL = StockFactory::createStock(("REGAL"));
       std::shared_ptr<Stock> GAMESTOP = StockFactory::createStock("GAMESTOP");
       std::shared_ptr<Stock> NVIDIA = StockFactory::createStock("NVIDIA");
       std::shared_ptr<Stock> TESLA = StockFactory::createStock("TESLA");
       std::shared_ptr<Stock> NOKIA = StockFactory::createStock("NOKIA");
       std::shared_ptr<Stock> MICROSOFT = StockFactory::createStock("MICROSOFT");
       std::shared_ptr<Stock> AMAZON = StockFactory::createStock("AMAZON");
       std::shared_ptr<Stock> APPLE = StockFactory::createStock("APPLE");

        REGAL->attach(bot);
        GAMESTOP->attach(bot);
        NVIDIA->attach(bot);
        TESLA->attach(bot);
        NOKIA->attach(bot);
        MICROSOFT->attach(bot);
        AMAZON->attach(bot);
        APPLE->attach(bot);

        stocks.push_back(REGAL);
        stocks.push_back(GAMESTOP);
        stocks.push_back(NVIDIA);
        stocks.push_back(TESLA);
        stocks.push_back(NOKIA);
        stocks.push_back(MICROSOFT);
        stocks.push_back(AMAZON);
        stocks.push_back(APPLE);
    }

    QString nextDay() {
        logger_->addDay();
        for (auto &stock : stocks) {
            stock->updatePrice();
        }

        emit stocksUpdated();
        updateBank();
        return Bot::getInstance().getBotBubble();
    }

    void addMoney(double chaching) {
        Bot::getInstance().deposit(chaching);
        updateBank();
    }

    void updateBank() {
        emit bankBalanceUpdated(Bot::getInstance().getBalance(),
                                Bot::getInstance().ownedStock,
                                Bot::getInstance().ownedStock2,
                                Bot::getInstance().ownedStock3);
    }

    std::vector<QString> endSimulation() {
        //sell remaining bots' stocks here
        Bot::getInstance().sellRemaining();

        return logger_->logSummary2();
    }

signals:
    void stocksUpdated();
    void bankBalanceUpdated(double newBalance, QString onee, QString twoo, QString threee);

private:
    std::vector<std::shared_ptr<Stock>> stocks;
    Outputter* logger_;
};

#endif // SIMULATIONMANAGER_H
