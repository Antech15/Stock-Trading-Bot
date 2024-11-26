#include "mainwindow.h"
#include "outputter.h"

#include <QApplication>
#include<QDir>

int main(int argc, char *argv[])
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    QString outputPath = dir.absolutePath() + "/bot_summary.txt";
    Outputter logger(outputPath);

    // Simulate some trades
    logger.logSell("Tech", 200.00);
    logger.logSell("Health", 200.00);
    logger.logSell("Tech", 200.00);
    logger.addDay();
    logger.addDay();

    QApplication a(argc, argv);
    MainWindow w;

    QObject::connect(&a, &QApplication::aboutToQuit, [&logger]() {
        logger.logSummary();
    });

    w.show();
    return a.exec();
}
