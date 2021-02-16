#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include "form.h"
namespace Ui {
class Widget;
}
class Form;

enum Type
{
    SHOWANSWER,
    CREATEENIGMA,

};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void setNum(int row, int col,int num);
    void clearNum(int row, int col);

protected:
    void process(bool show);

private slots:

    void on_showAnswerBtn_clicked();

    void on_createEnigmaBtn_clicked();


    void on_clearAllBtn_clicked();

private:
    Ui::Widget *ui;
    int _num[9][9];
    QPushButton _btn[9][9];
};

#endif // WIDGET_H
