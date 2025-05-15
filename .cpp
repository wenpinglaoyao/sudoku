#include "sukodugrid.h"
#include "ui_sukodugrid.h"

SukoduGrid::SukoduGrid(QWidget *parent, QPoint point, MainWindow *main, int row, int col) :
    QDialog(parent), _ui(new Ui::SukoduGrid)
{
    _ui->setupUi(this);
    this->setGeometry(point.x(),point.y(),this->width(),this->height());
    this->move(this->x()-this->width()/2,this->y()-this->height()/2);  // 这两行代码将弹出的九宫格定位到玩家光标点击的居中位置
    for (uint i=0;i<9;i++)                                             // 创建1~9的九宫格矩阵
    {
        QPushButton* btn = new QPushButton(this);                      // 创建一个按钮资源
        btn->setFont(QFont(nullptr,30));                               // 设置按钮字体size为30
        btn->setText(QString::number(i+1));                            // 设置对应的按钮数字
        connect(btn, &QPushButton::clicked, [this,main,row,col,i](){   // Qt按钮信号槽，使用lambda式来捕获上下文
            main->setNum(row,col,i+1);                                 // 调用主窗口的接口函数来设置对应数值
            this->close();                                             // 用过即丢
        });
        _ui->gridLayout->addWidget(btn,i/3,i%3);                       // 以3X3格式来布局按钮
    }
    connect(_ui->ClearNum,&QPushButton::clicked,[this,main,row,col](){ // 按钮的Qt信号槽注册
        main->clearNum(row,col);                                       // 擦除对应数字
        this->close();                                                 // 用过即丢
    });
}

SukoduGrid::~SukoduGrid()
{
    delete _ui;
}
