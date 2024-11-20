#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_chooseStocksButton_clicked();

    void on_bankButton_clicked();

    void on_nextDayButton_clicked();

    void on_goBackButton_clicked();

    void on_goBackButton_2_clicked();

    void on_dayLabel_linkActivated(const QString &link);

private:
    Ui::MainWindow *ui;
    int dayCounter = 0;

};
#endif // MAINWINDOW_H
