#ifndef OUTPUTTER_H
#define OUTPUTTER_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <map>

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
        stockTally[stockName] += 1;
        stockRevenue[stockName] += price;

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
            out << "For the " << stockNames[i] << " stock, you've purchased " << stockTally[stockNames[i]] << " shares. It gained you $"
                << stockRevenue[stockNames[i]] << "\n";

            total += stockRevenue[stockNames[i]];
        }
        out << "Total Bot Profit was $" << QString::number(total, 'f', 2) << "\n";

    }

    void addDay() {
        days++;
    }

private:
    QFile logFile;
    int days;
    std::map<QString, int> stockTally;
    std::map<QString, double> stockRevenue;
    std::vector<QString> stockNames;
};


#endif
