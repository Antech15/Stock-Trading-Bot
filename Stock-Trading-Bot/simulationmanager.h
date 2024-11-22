#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include "stock.h"
#include "bot.h"
#include <QObject>
#include <vector>

class SimulationManager : public QObject {
    Q_OBJECT

public:
    SimulationManager(QObject *parent = nullptr) : QObject(parent) {}

    void addStock(std::shared_ptr<Stock> stock) {
        stocks.push_back(stock);
    }

    void nextDay() {
        for (auto &stock : stocks) {
            stock->updatePrice();
        }
        emit stocksUpdated();
        emit bankBalanceUpdated(Bot::getInstance().getBalance());
    }

signals:
    void stocksUpdated();
    void bankBalanceUpdated(double newBalance);

private:
    std::vector<std::shared_ptr<Stock>> stocks;
};

#endif // SIMULATIONMANAGER_H
