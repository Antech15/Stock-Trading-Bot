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

int main() {
    // Create a trading facade
    TradingFacade facade;

    // Simulate a few days of trading
    facade.simulateNextDay();
    facade.performTrade();

    // Simulate another day of trading
    facade.simulateNextDay();
    facade.performTrade();

    return 0;
}
