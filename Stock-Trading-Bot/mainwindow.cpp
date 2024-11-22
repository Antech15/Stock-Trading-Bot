#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include<QFile>
#include "stockfactory.h"
#include "simulationmanager.h"
#include "bot.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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

    // Connect SimulationManager signals to MainWindow slots
    connect(simulationManager.get(), &SimulationManager::stocksUpdated, this, &MainWindow::updateStocks);
    connect(simulationManager.get(), &SimulationManager::bankBalanceUpdated, this, &MainWindow::updateBankBalance);

    // Create stocks and add to simulation
    std::shared_ptr<Bot> bot = std::shared_ptr<Bot>(&Bot::getInstance(), [](Bot*) {});

    Bot::getInstance().setStrategy(Bot::Strategy::Daily);   // For daily trading
    //Bot::getInstance().setStrategy(Bot::Strategy::BiDaily); // For bi-daily trading

    techStock = StockFactory::createStock(("Tech"));
    healthStock = StockFactory::createStock("Health");
    financeStock = StockFactory::createStock("Finance");
    energyStock = StockFactory::createStock("Energy");

    techStock->attach(bot);
    healthStock->attach(bot);
    financeStock->attach(bot);
    energyStock->attach(bot);

    simulationManager->addStock(techStock);
    simulationManager->addStock(healthStock);
    simulationManager->addStock(financeStock);
    simulationManager->addStock(energyStock);

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
        ui->stockLabel1->setText(QString("Tech: $%1").arg(stocks[0]->getPrice()));
        ui->stockLabel2->setText(QString("Health: $%1").arg(stocks[1]->getPrice()));
        ui->stockLabel3->setText(QString("Finance: $%1").arg(stocks[2]->getPrice()));
        ui->stockLabel4->setText(QString("Energy: $%1").arg(stocks[3]->getPrice()));
    }
}

void MainWindow::updateStocks() {
    ui->stockLabel1->setText(QString("Tech: $%1").arg(techStock->getPrice()));
    ui->stockLabel2->setText(QString("Health: $%1").arg(healthStock->getPrice()));
    ui->stockLabel3->setText(QString("Finance: $%1").arg(financeStock->getPrice()));
    ui->stockLabel4->setText(QString("Energy: $%1").arg(energyStock->getPrice()));
}


void MainWindow::updateBankBalance(double newBalance) {
    // Update the bank label with the new balance
    ui->bankLabel->setText(QString("Amount: $%1").arg(newBalance, 0, 'f', 2));
}

void MainWindow::on_chooseStocksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_bankButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_nextDayButton_clicked() {
    simulationManager->nextDay();
    dayCounter++;
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
