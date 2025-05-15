#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QDialog>
#include <QMessageBox>
#include "sukodugrid.h"

namespace Ui {class MainWindow;}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setNum(int,int,int);
    void clearNum(int,int);
protected:
    void process(bool);
private slots:
    void on_RandomLayout_clicked();
    void on_ClearLayout_clicked();
    void on_ViewSolution_clicked();

private:
    Ui::MainWindow *_ui;
    QPushButton _btn[9][9];
};

#endif // MAINWINDOW_H
