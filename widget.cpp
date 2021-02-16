#include "widget.h"
#include "ui_widget.h"
#include <time.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("数独小游戏"));
    this->setFixedSize(this->size());
    for(int row=0; row<9; row++)
        for(int col=0; col<9; col++)
        {
            _btn[row][col].setFixedSize(50,50);
            QFont font(nullptr,40);
            _btn[row][col].setFont(font);
            _btn[row][col].setText("");
            ui->gridLayout->addWidget(&_btn[row][col],row,col);

            connect(&_btn[row][col],&QPushButton::clicked,[this,row,col](){
               QPoint point = this->cursor().pos();//获取当前光标的位置
                Form* form = new Form(point,0,this,row,col);//创建一个小窗口
                form->exec();
            });
        }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::process(bool show)
{
    int num[9][9], flag[81], backarr[81][2];
    int i, r, c, rend, cend, back, layer, row, col, count;
    clock_t start, end;
    randstart:
        for (r = 0; r < 9;r++ )
            for (c = 0; c < 9; c++)
                num[r][c] = 0;
        for (i = 0; i < 81; i++)
                flag[i] = 0;
        for (r = 0; r < 81; r++)
            for (c = 0; c < 2; c++)
                backarr[r][c] = 0;
        i = r = c = rend = cend = back = layer = row = col = count = 0;//以上这些都是用来把所有的数据与变量清零，可以不用管它们

        if(show) //如果是解答局面的调用
        {
            for(int i=0; i<81; i++)
            {
                int value = _btn[i/9][i%9].text().toInt();
                if(value)
                {
                    num[i/9][i%9] = value;
                    flag[i] = 1;
                }
            }
        }else
        {
            for(int a=0; a<81; a++)
                _btn[a/9][a%9].setText("");
            srand((unsigned)(time)(NULL));//下面开始随机生成局面
            i = rand() % 16 + 14; //我们要生成的局面开始的固定数值最少14个，最多30个
            while (count < i)
            {
                layer = rand() % 81;
                if (!flag[layer])
                {
                    rend = rand() % 9 + 1;  //随机生产一个数字（1-9）

                    row = layer / 9; //定位行值与列值
                    col  = layer % 9;

                    for (r = 0; r < 9; r++)
                        if (num[r][col] == rend)
                            goto randend; //如果同列有相同数字了，那么1：继续循环，但count不自增

                    for (c = 0; c < 9; c++)
                        if (num[row][c] == rend)
                            goto randend; //如果同行有相同数字了，那么同1

                    for (int kl = ((layer / 9) / 3) * 3; kl < ((layer / 9) / 3) * 3 + 3; kl++)
                        for (int kh = ((layer % 9) / 3) * 3; kh < ((layer % 9) / 3) * 3 + 3; kh++)
                           if (num[kl][kh] == rend)
                               goto randend; //如果九宫域有相同数字了，那么同1和2

                    flag[layer] = 1; //标记上这一块是石头数，后面算法不要动它
                    num[row][col] = rend; //把随机生产的数字投放到这个位置中
                    count++;
                }
            randend:;
            }
            //到这里局面已生成

            for (r = 0; r < 9; r++)
            {
                for (c = 0; c < 9; c++)
                {
                    if (num[r][c] != 0)
                        _btn[r][c].setText(QString::number(num[r][c]));
                }
            }
        }



        i = r = c = rend = cend = back = layer = row = col = count = 0; //变量清零，方便后来的算法思考

        start = clock(); //记录下程序开始“思考”的时间
        while (layer < 81) //因为要测试81次，layer代表81层的每一层
        {
            if (layer < 0) //如果这个局面是无解的
            {
                if(!show)
                {
                    goto randstart;
                }
                else
                {
                    QMessageBox::warning(0,tr("警告"),tr("这是一个无解的死局局面，请检查！"));
                    ui->showAnswerBtn->setEnabled(true);
                    return;
                }

            }
            if (1 == flag[layer]) //这个分支用来测试是否计算到了固定数值，如果是，则跳过这一块
            {
                if (back == 1) { goto no_match; } //如果正处于回溯状态，就当成无满足条件的递归式回到上一层
                layer++; // 不然则当成前进式计算下一层
                continue;
            }

            row = layer / 9; col = layer % 9; //把这一层用来转换定位行和列的具体坐标

            for (i = backarr[layer][back] + 1; i < 10; i++) //注意那个backarr，它用来当作测试数值的下限值
            {
                for (c = 0; c < 9; c++)//检查横列
                if (c >= col)
                {
                    if (flag[row * 9 + c] == 1 && num[row][c] == i)  goto end;//i不符合条件，刷新i
                }
                else if (num[row][c] == i)  { goto end; }

                for (r = 0; r < 9; r++) //检查竖列
                if (r >= row)
                {
                    if (flag[r * 9 + col] == 1 && num[r][col] == i)  goto end;
                }
                else if (num[r][col] == i)  goto end;

                rend = ((row / 3) * 3) + 3; cend = ((col / 3) * 3) + 3; //检查九宫格
                for (r = (row / 3) * 3; r < rend; r++)
                for (c = (col / 3) * 3; c < cend; c++)
                if ((r * 9 + c) >= layer)
                {
                    if (flag[r * 9 + c] == 1 && num[r][c] == i)  goto end;
                }
                else if (num[r][c] == i)  goto end;


                goto match_go;

            end:continue;
            }
            goto no_match;

        match_go:
            num[row][col] = i; //i暂时符合限制条件，程序继续前进式计算
            backarr[layer][1] = i; //设置这一层的下限值，不然回溯时算法会进入死循环
            back = 0;
            layer++;
            continue;

        no_match:
            back = 1; //没有符合条件的数值，将状态职位回溯
            layer--;
        }
        //程序运行到这里表示已经找到解答了
        end = clock(); //这个与那个start配合，是用来计算程序思考的用时（不过这里并没有使用）


        if(show) //如果是要求显示答案的调用请求
        {
            for (r = 0; r < 9; r++)
            {
                for (c = 0; c < 9; c++)
                {
                    if (num[r][c] != 0)
                    {
                        _btn[r][c].setText(QString::number(num[r][c]));
                    }
                }
            }
            ui->showAnswerBtn->setEnabled(true);
        }else
            ui->createEnigmaBtn->setEnabled(true);
}

void Widget::setNum(int row, int col, int num)//用num数字，设置为row行col列块的数字
{
    int value = num+1;
    for(int c=0; c<9; c++)
        if(value == _btn[row][c].text().toInt())
        {
            QMessageBox::warning(0,tr("警告"),tr("在同列中有重复数字，请检查！"));
            return;
        }
    for(int r=0; r<9; r++)
        if(value == _btn[r][col].text().toInt())
        {
            QMessageBox::warning(0,tr("警告"),tr("在同行中有重复数字，请检查！"));
            return;
        }
    for(int r=0; r<3; r++)
        for(int c=0; c<3; c++)
            if(value == _btn[row/3*3+r][col/3*3+c].text().toInt())
            {
                QMessageBox::warning(0,tr("警告"),tr("在同九宫域中有重复数字，请检查！"));
                return;
            }
    _btn[row][col].setText(QString::number(value));
}
void Widget::clearNum(int row, int col) //擦除这一个块的数字
{
    _btn[row][col].setText("");
}

void Widget::on_showAnswerBtn_clicked()
{
    ui->showAnswerBtn->setEnabled(false);
    this->process(1);
}

void Widget::on_createEnigmaBtn_clicked()
{
    ui->createEnigmaBtn->setEnabled(false);
    this->process(0);
}

void Widget::on_clearAllBtn_clicked()
{
    for(int row=0; row<9; row++)
        for(int col=0; col<9; col++)
            _btn[row][col].setText("");
}
