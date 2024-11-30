#ifndef OUTPUTTER_H
#define OUTPUTTER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <map>
#include <iostream>

class Outputter {
public:
    Outputter(const QString &fileName)
        : logFile(fileName) {
        qDebug() << "Attempting to open file:" << fileName;
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open log file:" << fileName;
            qDebug() << "Error:" << logFile.errorString();
        } else {
            qDebug() << "File opened successfully at:" << logFile.fileName();
        }
        days = 0;
    }

    ~Outputter() {
        if (logFile.isOpen()) {
            logFile.close();
        }
    }

    void logSell(const QString &stockName, double price) {
        if(price>0){
            stockTally[stockName] += 1;
        }
        stockTally[stockName] += 1;
        stockRevenue[stockName] += price;

        qDebug() << "Logged a sell for " << stockName << " with a profit of " << price;

        for(std::size_t i = 0; i < stockNames.size(); i++) {
            if(stockName == stockNames[i])
                return;
        }
        stockNames.push_back(stockName);
    }

    void logSummary() {
        if (!logFile.isOpen()) return;

        double total = 0;

        QTextStream out(&logFile);
        out << "Bot Summary:\n";
        out << "You traded for: " << days << " days. \n";
        for (std::size_t i = 0; i < stockNames.size(); i++) {
            out << "For the " << stockNames[i] << " stock, you've sold " << QString::number(stockTally[stockNames[i]]) << " share(s). It gained you $"
                << stockRevenue[stockNames[i]] << "\n";

            total += stockRevenue[stockNames[i]];
        }
        out << "Total Bot Profit was $" << QString::number(total, 'f', 2) << "\n";

    }

    std::vector<QString> logSummary2() {
        double total = 0;

        std::vector<QString> vec;
        vec.push_back("You traded for: " + QString::number(days) + " days. \n");
        for (std::size_t i = 0; i < stockNames.size(); i++) {
            vec.push_back("For the "+ stockNames[i] + " stock, you've sold " + QString::number(stockTally[stockNames[i]]) + " share(s). It gained you $"
                + QString::number(stockRevenue[stockNames[i]]) + "\n");

            total += stockRevenue[stockNames[i]];
        }
        vec.push_back("Total Bot Profit was $" + QString::number(total, 'f', 2) + "\n");

        return vec;
    }

    void addDay() {
        days++;
    }

    double getTally(QString stockName) {
        return stockTally[stockName];
    }

private:
    QFile logFile;
    int days;
    std::map<QString, int> stockTally;
    std::map<QString, double> stockRevenue;
    std::vector<QString> stockNames;
};
#endif
