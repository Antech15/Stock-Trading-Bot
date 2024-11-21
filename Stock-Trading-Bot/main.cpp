/*#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}*/
// main.cpp
#include <iostream>
#include "Facade/TradingFacade.h"
#include "TradingBot.h"
#include "Strategy/RiskAverseStrategy.h"
#include "Strategy/GrowthStrategy.h"

int main() {
    TradingFacade facade;

    // Create the TradingBot
    TradingBot* bot = new TradingBot();

    // Set the trading strategy
    bot->setStrategy(new RiskAverseStrategy());  // Set to RiskAverseStrategy or GrowthStrategy

    // Add the TradingBot as an observer to the facade
    facade.simulateNextDay();

    // Simulate the trading bot action
    bot->update();

    // Cleanup
    delete bot;

    return 0;
}
