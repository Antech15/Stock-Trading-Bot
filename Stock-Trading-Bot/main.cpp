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

    QApplication a(argc, argv);
    QString styleSheet = "QWidget { background-color: #e9e6d1; }"  //Light grey background
                         "QPushButton { background-color: #5f714b; color: #e9e6d1; }"  //Blue button with white text
                         "QLabel { color: #2c3e50; font-size: 16px; }";  //Dark text for labels
    a.setStyleSheet(styleSheet);
    MainWindow w(logger);

    QObject::connect(&a, &QApplication::aboutToQuit, [&logger]() {
        logger.logSummary();
    });

    w.show();
    return a.exec();
}
