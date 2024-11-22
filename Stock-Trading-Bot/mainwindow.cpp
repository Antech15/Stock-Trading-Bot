#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include<QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_chooseStocksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_bankButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_nextDayButton_clicked()
{
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
