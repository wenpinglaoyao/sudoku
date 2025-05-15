#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),_ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    this->setWindowTitle(tr("作者：范效萌"));
    for(uint row=0;row<9;row++)
        for(uint col=0;col<9;col++){
            if(row/3 %2 == col/3%2)
                _btn[row][col].setStyleSheet("background-color: rgb(0,200,200)");
            else
                _btn[row][col].setStyleSheet("background-color: rgb(200,200,0)");
            _btn[row][col].setFont(QFont(nullptr,40));
            connect(&_btn[row][col],&QPushButton::clicked, [this,row,col](){
                QPoint point = this->cursor().pos();
                SukoduGrid* grid = new SukoduGrid(0,point,this,row,col);
                grid->exec();
            });
            _ui->gridLayout->addWidget(&_btn[row][col],row,col);
        }
}
MainWindow::~MainWindow(){delete _ui;}

void MainWindow::setNum(int row, int col, int num)//设置row行col列块的数字的数值为num
{
    for(int c=0; c<9; c++)   // 检测同一个行里，有没有重复的数字，若有，那么弹出警告框告诉玩家设置失败
        if(num == _btn[row][c].text().toInt())
        {
            QMessageBox::warning(0,tr("警告"),tr("在同列中有重复数字，请检查！"));
            return;
        }
    for(int r=0; r<9; r++)   // 同上，检测同列
        if(num == _btn[r][col].text().toInt())
        {
            QMessageBox::warning(0,tr("警告"),tr("在同行中有重复数字，请检查！"));
            return;
        }
    for(int r=0; r<3; r++)   // 同上，检查同九宫格
        for(int c=0; c<3; c++)
            if(num == _btn[row/3*3+r][col/3*3+c].text().toInt())
            {
                QMessageBox::warning(0,tr("警告"),tr("在同九宫域中有重复数字，请检查！"));
                return;
            }
    _btn[row][col].setText(QString::number(num));  // 到这里说明行、列、九宫格没有重复数字，那么设置成功
}
void MainWindow::clearNum(int row, int col) // 擦除某个按钮的数字，row为行坐标，col为列坐标
{
    _btn[row][col].setText("");
}

void MainWindow::on_RandomLayout_clicked()  // 随机生成一个新谜题
{
    _ui->RandomLayout->setEnabled(false);
    this->process(0);
}

void MainWindow::on_ClearLayout_clicked()   // 清除局面的所有按钮上的数字
{
    for(uint row=0;row<9;row++)
        for(uint col=0;col<9;col++)
            _btn[row][col].setText("");
}

void MainWindow::on_ViewSolution_clicked()  // 查看当前谜题的解法
{
    _ui->ViewSolution->setEnabled(false);
    this->process(true);
}

void MainWindow::process(bool show)
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
        i = r = c = rend = cend = back = layer = row = col = count = 0;// 以上这些都是用来把所有的数据与变量清零，可以不用管它们

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
            } //到这里局面已生成

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
        while (layer < 81) //因为要迭代81次，layer代表81层的每一层
        {
            if (layer < 0) //回归破0即表示这个局面是无解的
            {
                if(!show)
                {
                    goto randstart;
                }
                else
                {
                    QMessageBox::warning(0,tr("警告"),tr("这是一个无解的死局局面，请检查！"));
                    _ui->ViewSolution->setEnabled(true);
                    return;
                }

            }
            if (1 == flag[layer]) //这个分支用来测试是否计算到了固定数值，若是则跳过
            {
                if (back == 1) { goto no_match; } // 若程序正处于回溯状态，那就当成无满足条件，递归式回归到上一层
                layer++;                          // 不然则当成前进式计算下一层
                continue;
            }

            row = layer / 9; col = layer % 9;     // 把这一层用来转换定位行和列的具体坐标

            for (i = backarr[layer][back] + 1; i < 10; i++) // 注意那个backarr，它用来当作测试数值的下限值
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

            end:;                  // 不符合条件，继续检查下一个数字
            }
            goto no_match;         // 没有符合条件的数值，进入回溯状态

        match_go:
            num[row][col] = i;     // i暂时符合限制条件，设置为当前数值，程序继续前进式计算
            backarr[layer][1] = i; // 设置这一层的下限值，不然回溯时算法会进入死循环
            back = 0;              // 清除回溯状态
            layer++;               // 继续下一层
            continue;

        no_match:
            back = 1;  //没有符合条件的数值，将状态置为回溯
            layer--;
        }              //程序运行到这里表示已经找到解答了
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
            _ui->ViewSolution->setEnabled(true); // 启用【查看解法】按钮
        }else
            _ui->RandomLayout->setEnabled(true); // 启用【随机生成谜题】按钮
}
