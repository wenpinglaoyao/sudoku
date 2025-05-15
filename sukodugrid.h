#ifndef SUKODUGRID_H
#define SUKODUGRID_H

#include "mainwindow.h"

namespace Ui {
class SukoduGrid;
}
class MainWindow;
class SukoduGrid : public QDialog
{
    Q_OBJECT

public:
    explicit SukoduGrid(QWidget *parent,QPoint, MainWindow*, int,int);
    ~SukoduGrid();

private:
    Ui::SukoduGrid *_ui;
};

#endif // SUKODUGRID_H
