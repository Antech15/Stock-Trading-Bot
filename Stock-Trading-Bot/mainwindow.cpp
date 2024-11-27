#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include<QFile>
#include "stockfactory.h"
#include "simulationmanager.h"
#include "bot.h"

MainWindow::MainWindow(Outputter &logger, QWidget *parent)
    : QMainWindow(parent), logger(logger)
    , ui(new Ui::MainWindow)
    , simulationManager(new SimulationManager)
{
    ui->setupUi(this);

    QFile file(":/images/images/yapdolla.png");
    if (!file.exists()) {
        qDebug() << "File does not exist at the specified path";
    } else {
        QPixmap pix{":/images/images/yapdolla.png"};
        pix = pix.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(pix);
    }

    QFile file2(":/images/images/speech-bubble-png-15286.png");
    if (!file2.exists()) {
        qDebug() << "File does not exist at the specified path";
    } else {
        QPixmap pix{":/images/images/speech-bubble-png-15286.png"};
        pix = pix.scaled(ui->speechLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->speechLabel->setPixmap(pix);
    }

    ui->stackedWidget->setCurrentIndex(1);

    // Connect SimulationManager signals to MainWindow slots
    connect(simulationManager.get(), &SimulationManager::stocksUpdated, this, &MainWindow::updateStocks);
    connect(simulationManager.get(), &SimulationManager::bankBalanceUpdated, this, &MainWindow::updateBankBalance);

    // Create stocks and add to simulation
    std::shared_ptr<Bot> bot = std::shared_ptr<Bot>(&Bot::getInstance(), [](Bot*) {});
    Bot::getInstance().attatchLogger(&logger);

    //Bot::getInstance().setStrategy(Bot::Strategy::Daily);   // For daily trading
    //Bot::getInstance().setStrategy(Bot::Strategy::Weekly); // For bi-daily trading

    REGAL = StockFactory::createStock(("REGAL"));
    GAMESTOP = StockFactory::createStock("GAMESTOP");
    NVIDIA = StockFactory::createStock("NVIDIA");
    TESLA = StockFactory::createStock("TESLA");
    NOKIA = StockFactory::createStock("NOKIA");
    MICROSOFT = StockFactory::createStock("MICROSOFT");
    AMAZON = StockFactory::createStock("AMAZON");
    APPLE = StockFactory::createStock("APPLE");




    REGAL->attach(bot);
    GAMESTOP->attach(bot);
    NVIDIA->attach(bot);
    TESLA->attach(bot);
    NOKIA->attach(bot);
    MICROSOFT->attach(bot);
    AMAZON->attach(bot);
    APPLE->attach(bot);


    simulationManager->addStock(REGAL);
    simulationManager->addStock(GAMESTOP);
    simulationManager->addStock(NVIDIA);
    simulationManager->addStock(TESLA);
    simulationManager->addStock(NOKIA);
    simulationManager->addStock(MICROSOFT);
    simulationManager->addStock(AMAZON);
    simulationManager->addStock(APPLE);


    initializeStockPrices();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeStockPrices() {
    // Assuming you use QLabel widgets like stockLabel1, stockLabel2, etc.
    auto stocks = simulationManager->getStocks(); // Add a getter for stocks in SimulationManager

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

void MainWindow::updateStocks() {
    ui->stockLabel1->setText(QString("REGAL: $%1").arg(REGAL->getPrice()));
    ui->stockLabel2->setText(QString("GAMESTOP: $%1").arg(GAMESTOP->getPrice()));
    ui->stockLabel3->setText(QString("NVIDIA: $%1").arg(NVIDIA->getPrice()));
    ui->stockLabel4->setText(QString("TESLA: $%1").arg(TESLA->getPrice()));
    ui->stockLabel4->setText(QString("NOKIA: $%1").arg(NOKIA->getPrice()));
    ui->stockLabel4->setText(QString("MICROSOFT: $%1").arg(MICROSOFT->getPrice()));
    ui->stockLabel4->setText(QString("AMAZON: $%1").arg(AMAZON->getPrice()));
    ui->stockLabel4->setText(QString("APPLE: $%1").arg(APPLE->getPrice()));



}


void MainWindow::updateBankBalance(double newBalance) {
    // Update the bank label with the new balance
    ui->bankLabel->setText(QString("Amount: $%1").arg(newBalance, 0, 'f', 2));
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
    simulationManager->nextDay();
    dayCounter++;

    QString temp = Bot::getInstance().getBotBubble();
    ui->consoleLabel->setText(temp);

    //I think make the string a variable and retreieve it here like in lines 83-86
    ui->consoleLabel->setStyleSheet("color: black;");
    ui->dayLabel->setText(QString("Day: %1").arg(dayCounter));

    logger.addDay();
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
    Daily *dailyStrategy = new Daily();   // Use Daily strategy
    Bot::getInstance().setStrategy(dailyStrategy); // Setting Daily strategy

    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_weeklyButton_clicked()
{

    Weekly *weeklyStrategy = new Weekly();   // Use Daily strategy
    Bot::getInstance().setStrategy(weeklyStrategy); // Setting Daily strategy
    ui->stackedWidget->setCurrentIndex(0);

}


void MainWindow::on_tenButton_clicked()
{
    Bot::getInstance().deposit(10.0);

    double balance = Bot::getInstance().getBalance();

    updateBankBalance(balance);
}


void MainWindow::on_twentyButton_clicked()
{
    Bot::getInstance().deposit(20.0);

    double balance = Bot::getInstance().getBalance();

    updateBankBalance(balance);
}


void MainWindow::on_fiftyButton_clicked()
{
    Bot::getInstance().deposit(50.0);

    double balance = Bot::getInstance().getBalance();

    updateBankBalance(balance);
}


void MainWindow::on_hundredButton_clicked()
{
    Bot::getInstance().deposit(100.0);

    double balance = Bot::getInstance().getBalance();

    updateBankBalance(balance);
}


void MainWindow::on_doneButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

