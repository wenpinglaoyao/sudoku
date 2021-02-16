#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPushButton>
#include "widget.h"

namespace Ui {
class Form;
}
class Widget;

class Form : public QDialog
{
    Q_OBJECT

public:
    explicit Form(QPoint point, QWidget *parent = 0,Widget* widget=0, int row=0, int col=0);
    ~Form();

private slots:
    void on_clearNumBtn_clicked();

private:
    Ui::Form *ui;
    QPushButton _btn[9];
    Widget* _wid;
    int _row,_col;
    QPoint _point;
};

#endif // FORM_H
