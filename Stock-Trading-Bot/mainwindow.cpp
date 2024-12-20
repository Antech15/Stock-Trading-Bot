#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include<QFile>
#include "simulationmanager.h"

MainWindow::MainWindow(Outputter &logger, QWidget *parent)
    : QMainWindow(parent), logger(logger)
    , ui(new Ui::MainWindow)
    , simulationManager(new SimulationManager)
{
    ui->setupUi(this);
    ui->consoleLabel->setStyleSheet("color: black; background: transparent;");
    ui->bankLabel_3->setStyleSheet("background: transparent;");

    //dolla image
    QPixmap pix{":/images/images/yapdolla.png"};
    pix = pix.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(pix);

    //speech bubble image
    QPixmap pix2{":/images/images/speech-bubble-png-15286.png"};
    pix = pix2.scaled(ui->speechLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->speechLabel->setPixmap(pix);
    ui->speechLabel->setStyleSheet("background: transparent;");

    //regal logo
    QPixmap pix3{":/images/images/regal.png"};
    pix = pix3.scaled(ui->regalView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->regalView->setPixmap(pix);

    //gamestop logo
    QPixmap pix4{":/images/images/gamestop.png"};
    pix = pix4.scaled(ui->gamestopView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->gamestopView->setPixmap(pix);

    //nvidia logo
    QPixmap pix5{":/images/images/nvidia.png"};
    pix = pix5.scaled(ui->nvidiaView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->nvidiaView->setPixmap(pix);

    //tesla logo
    QPixmap pix6{":/images/images/tesla.png"};
    pix = pix6.scaled(ui->teslaView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->teslaView->setPixmap(pix);

    //nokia logo
    QPixmap pix7{":/images/images/nokia.png"};
    pix = pix7.scaled(ui->nokiaView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->nokiaView->setPixmap(pix);

    //microsoft logo
    QPixmap pix8{":/images/images/microsoft.png"};
    pix = pix8.scaled(ui->microsoftView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->microsoftView->setPixmap(pix);

    //amazon logo
    QPixmap pix9{":/images/images/amazon.png"};
    pix = pix9.scaled(ui->amazonView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->amazonView->setPixmap(pix);

    //apple logo
    QPixmap pix10{":/images/images/apple.png"};
    pix = pix10.scaled(ui->appleView->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->appleView->setPixmap(pix);

    ui->stackedWidget->setCurrentIndex(1);

    // Connect SimulationManager signals to MainWindow slots
    connect(simulationManager.get(), &SimulationManager::stocksUpdated, this, &MainWindow::updateStocks);
    connect(simulationManager.get(), &SimulationManager::bankBalanceUpdated, this, &MainWindow::updateBankBalance);

    updateStocks();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStocks() {
    auto stocks = simulationManager->getStocks();

    if (!stocks.empty()) {
        ui->stockLabel1->setText(QString("REGAL: $%1").arg(stocks[0]->getPrice()));
        ui->stockLabel2->setText(QString("GAMESTOP: $%1").arg(stocks[1]->getPrice()));
        ui->stockLabel3->setText(QString("NVIDIA: $%1").arg(stocks[2]->getPrice()));
        ui->stockLabel4->setText(QString("TESLA: $%1").arg(stocks[3]->getPrice()));
        ui->stockLabel5->setText(QString("NOKIA: $%1").arg(stocks[4]->getPrice()));
        ui->stockLabel6->setText(QString("MICROSOFT: $%1").arg(stocks[5]->getPrice()));
        ui->stockLabel7->setText(QString("AMAZON: $%1").arg(stocks[6]->getPrice()));
        ui->stockLabel8->setText(QString("APPLE: $%1").arg(stocks[7]->getPrice()));
    }
}

void MainWindow::updateBankBalance(double newBalance, QString onee, QString twoo, QString threee) {
    QString temp = QString("Amount: $%1").arg(newBalance, 0, 'f', 2);
    auto stocks = simulationManager->getStocks();

    double total = newBalance;

    if (!onee.isEmpty()) {
        double price = 0;
        for (auto &stock : stocks) {
            if(stock->getName() == onee)
                price = stock->getPrice();
        }
        temp += QString("\n\n your %1 share is worth: $%2").arg(onee).arg(price, 0, 'f', 2);
        total += price;
    }

    if (!twoo.isEmpty()) {
        double price = 0;
        for (auto &stock : stocks) {
            if(stock->getName() == twoo)
                price = stock->getPrice();
        }
        temp += QString("\n\n your %1 share is worth: $%2").arg(twoo).arg(price, 0, 'f', 2);
        total += price;
    }

    if (!threee.isEmpty()) {
        double price = 0;
        for (auto &stock : stocks) {
            if(stock->getName() == threee)
                price = stock->getPrice();
        }
        temp += QString("\n\n your %1 share is worth: $%2").arg(threee).arg(price, 0, 'f', 2);
        total += price;
    }

    if (total != newBalance)
        temp += QString("\n\n Total bank and assets values is: $%1").arg(total, 0, 'f', 2);

    ui->bankLabel->setText(temp);
}

void MainWindow::on_chooseStocksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_bankButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_nextDayButton_clicked() {
    QString temp = simulationManager->nextDay();
    dayCounter++;

    ui->consoleLabel->setText(temp);
    ui->consoleLabel->setStyleSheet("color: black; background: transparent;");
    ui->dayLabel->setText(QString("Day: %1").arg(dayCounter));
}

void MainWindow::on_goBackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_goBackButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_dailyButton_clicked()
{
    simulationManager->startSimulation(logger, "dailyStrategy");
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_weeklyButton_clicked()
{
    simulationManager->startSimulation(logger, "lightMomentumStrategy");
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_tenButton_clicked()
{
    simulationManager->addMoney(10.0);
}

void MainWindow::on_twentyButton_clicked()
{
    simulationManager->addMoney(20.0);
}

void MainWindow::on_fiftyButton_clicked()
{
    simulationManager->addMoney(50.0);
}

void MainWindow::on_hundredButton_clicked()
{
    simulationManager->addMoney(100.0);
}

void MainWindow::on_doneButton_clicked()
{
    std::vector<QString> array = simulationManager->endSimulation();

    QString text;
    for (const QString &line : array) {
        text += line;
    }

    ui->summaryLabel->setText(text);
    ui->summaryLabel->setAlignment(Qt::AlignCenter);
    QFont font = ui->summaryLabel->font();
    font.setPointSize(14);
    ui->summaryLabel->setFont(font);

    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_hMomentumButton_clicked()
{
    simulationManager->startSimulation(logger, "hmomentumStrategy");
    ui->stackedWidget->setCurrentIndex(0);
}

