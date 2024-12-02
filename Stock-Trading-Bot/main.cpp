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
    // Apply a global stylesheet
    QString styleSheet = "QWidget { background-color: #f0f0f0; }"  // Light grey background
                         "QPushButton { background-color: #3498db; color: white; }"  // Blue button with white text
                         "QLabel { color: #2c3e50; font-size: 16px; }";  // Dark text for labels
    a.setStyleSheet(styleSheet);
    MainWindow w(logger);

    QObject::connect(&a, &QApplication::aboutToQuit, [&logger]() {
        logger.logSummary();
    });

    w.show();
    return a.exec();
}
