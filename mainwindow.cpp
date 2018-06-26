#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = new QTableWidget(20,3,this);
    ui->horizontalLayout->addWidget(table);
    table->set
    //this->S1.test_01();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::treeVisualization(){

}

void MainWindow::on_pushButtonTest01_clicked()
{
    this->S1.test_01();
}

void MainWindow::on_pushButtonTest02_clicked()
{
    this->S1.test_02();
}

void MainWindow::on_pushButtonTest03_clicked()
{
    this->S1.test_03();
}
