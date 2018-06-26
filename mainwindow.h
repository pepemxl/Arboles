#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <searchtree.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    searchtree S1;
    QTableWidget *table;

    void treeVisualization();
private slots:

    void on_pushButtonTest01_clicked();

    void on_pushButtonTest02_clicked();

    void on_pushButtonTest03_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
