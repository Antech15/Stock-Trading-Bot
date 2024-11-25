#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "simulationmanager.h"
#include "stock.h" // Include Stock class

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void updateStocks();
    void updateBankBalance(double newBalance);

    void on_chooseStocksButton_clicked();
    void on_bankButton_clicked();
    void on_nextDayButton_clicked();
    void on_goBackButton_clicked();
    void on_goBackButton_2_clicked();

private:
    Ui::MainWindow *ui;
    int dayCounter = 0;
    std::unique_ptr<SimulationManager> simulationManager;

    // Declare stock variables as members
    std::shared_ptr<Stock> techStock;
    std::shared_ptr<Stock> healthStock;
    std::shared_ptr<Stock> financeStock;
    std::shared_ptr<Stock> energyStock;

    void initializeStockPrices();
};

#endif // MAINWINDOW_H
