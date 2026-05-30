#include "GameWindow.h"
#include <QGridLayout>
#include "../entity/Hero/Knight.h"
#include <QPixmap>
#include <QVBoxLayout>
#include "PieceWidget.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 状态记录参数的初始化
    isDragging = false;
    dragFromRow = -1;
    dragFromCol = -1;
    dragFromPos = -1;
    dragHero = nullptr;

    // 1. 初始化逻辑层 Board
    board = std::make_unique<Board>(8, 8, 8);

    //------测试--------//
    board->placeUnitAt(3, 3, new Knight("test 1", 3, 3,1));
    board->placeUnitAt(2,  new Knight("test 2", 2, 2));
    board->placeUnitAt(4,  new Knight("test 3", 4, 4));
    //--------------//

    //--------------------------下面就是画图的部分-----------------------------//
    // 1. 设置窗口基础属性
    setWindowTitle("my_Arena");
    resize(600, 800); // 稍微调高一点窗口，给上下备战区留出足够空间

    // 2. 设置中央部件
    QWidget* windowWidget = new QWidget(this);
    //为窗口大控件设置布局方式
    QVBoxLayout *windowVLayout = new QVBoxLayout(windowWidget); // 改用垂直布局
    windowVLayout->setSpacing(20); // 设置上下两部分之间的间距（空气感）
    windowVLayout->setContentsMargins(20, 20, 20, 20); // 设置整体边距
    //然后把这个窗口主控件设置为中央控件件（强制规定）
    setCentralWidget(windowWidget);

    // ------------------- 第一部分：中间棋盘 -----------------
    // 我们先用一个控件包含棋盘部分
    QWidget* boardPart = new QWidget(this);
    boardPart->setStyleSheet("border: 1px solid white;background:white");
    boardPart->setFixedSize((CELL_SIZE+10)*board->getCols(), (CELL_SIZE+20)*board->getRows());
    //设置垂直布局和基本样式
    QGridLayout *boardLayout = new QGridLayout(boardPart);
    boardLayout->setContentsMargins(10, 10, 10, 10);
    boardLayout->setSpacing(4);

    for (int r = 0; r < board->getRows(); ++r) {
        // Data:每行的格子容器vector，临时容器，负责最后塞到成员中
        std::vector<PieceWidget*> rowLabels;
        for (int c = 0; c < board->getCols(); ++c) {
            //把网格虚拟布局的每个格子的位置放一个控件，显示出格子的形态
            QWidget* cell = new QWidget(this);
            cell->setFixedSize(CELL_SIZE, CELL_SIZE+8);
            if (r<board->getRows()/2) {
                cell->setStyleSheet("background:#f0b9b4");
            }
            else {
                cell->setStyleSheet("border: 1px solid #ebc996;");
            }

            //在cell内部放一个垂直布局器,负责放子控件
            QVBoxLayout *cellLayout = new QVBoxLayout(cell);
            cellLayout->setContentsMargins(0, 0, 0, 0);
            //把我的子控件放进去
            PieceWidget* pieceWidget = new PieceWidget(this);
            cellLayout->addWidget(pieceWidget, 0, Qt::AlignCenter);
            //把这个cell子控件放到网格布局器中
            boardLayout->addWidget(cell,r,c);

            // Data:把一行的每个网格放入上面定义的临时容器中
            rowLabels.push_back(pieceWidget);
        }
        //收集完一行的每一个之后，放到网格记录器中
        gridWidgets.push_back(rowLabels);
    }
    // 将棋盘加入主垂直布局
    boardLayout->setAlignment(Qt::AlignHCenter);
    windowVLayout->addWidget(boardPart,0,Qt::AlignCenter);

    // --------------第二部分：下方备战区 ----------------
    QWidget* benchPart = new QWidget(this);
    benchPart->setStyleSheet("background:white;border:1px solid ;");
    // 设置水平布局
    QHBoxLayout *benchLayout = new QHBoxLayout(benchPart);
    benchLayout->setSpacing(5);

    for (int c = 0; c < board->getBen(); ++c) {
        // 创建单个格子控件
        QWidget* cell = new QWidget();
        cell->setFixedSize(CELL_SIZE, CELL_SIZE + 8);
        cell->setStyleSheet("border: 1px solid gray; background: #fff7f0;");
        //给每个格子创建垂直布局
        QVBoxLayout* cellLayout = new QVBoxLayout(cell);
        cellLayout->setContentsMargins(0, 0, 0, 0);
        // 创建棋子控件，并把棋子控件放入格子的布局
        PieceWidget* pieceWidget = new PieceWidget(cell);
        cellLayout->addWidget(pieceWidget, 0, Qt::AlignCenter);
        // 把整个格子加入水平布局
        benchLayout->addWidget(cell);

        // Data: 记录
        benchWidgets.push_back(pieceWidget);
    }
    benchLayout->setAlignment(Qt::AlignHCenter);
    windowVLayout->addWidget(benchPart, 0,Qt::AlignCenter);

    //---------------------第三部分，画下方的开始结束按钮---------------------//
    //大框控件
    QWidget* controlButtonPart = new QWidget(this);
    controlButtonPart->setStyleSheet("border:1px solid gray;background:white");
    controlButtonPart->setFixedSize(CELL_SIZE*board->getBen(), CELL_SIZE+8);
    QHBoxLayout* controlButtonLayout = new QHBoxLayout(controlButtonPart);
    // 第一个开始战斗按钮
    QPushButton* combatButton = new QPushButton("Combat", controlButtonPart); // 直接传入文字和父控件
    combatButton->setFixedSize(70, 40);
    combatButton->setStyleSheet("color:red;border: 1px solid gray; background:white ;");
    connect(combatButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "combatButtonClicked";

    });

    // 第二个返回备战按钮
    QPushButton* prepareButton = new QPushButton("Return", controlButtonPart);
    prepareButton->setFixedSize(70, 40);
    prepareButton->setStyleSheet("color:red; border: 1px solid gray; background: white;");

    //添加到水平布局中
    controlButtonLayout->addWidget(prepareButton);
    controlButtonLayout->addWidget(combatButton);
    controlButtonLayout->setAlignment(Qt::AlignCenter);
    //将水平布局添加到窗口布局中
    windowVLayout->addWidget(controlButtonPart,1,Qt::AlignCenter);
    //------------------------画图部分结束-----------------------------//

    // 刷新棋盘显示
    this->updateBoardUI();
}


// 鼠标按下事件：负责“选中”棋子
void GameWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 获取鼠标点击位置的子控件
        QWidget *clickedWidget = childAt(event->pos());
        if (!clickedWidget) return;
        // std::cout<<"catch click"<<std::endl;
        // 遍历棋盘，判断是否点中了某个有棋子的格子
        for (int r = 0; r < board->getRows(); ++r) {
            for (int c = 0; c < board->getCols(); ++c) {
                if (gridWidgets[r][c] == clickedWidget && !board->isCellEmpty(r, c) && board->getUnitAt(r,c)->getOwner()==0) {
                    // 选中成功！记录状态
                    isDragging = true;
                    dragFromRow = r;
                    dragFromCol = c;
                    dragHero= board->getUnitAt(r,c);
                    // std::cout<< "choose successfully, update the parameter"<<std::endl;
                    // std::cout<<"parameter: "<< dragFromRow<<" , "<<dragFromCol<<" , "<<dragFromPos<<std::endl;
                    gridWidgets[r][c]->setStyleSheet("background-color: #ffffcc; border: 3px solid yellow;");
                    return;
                }
            }
        }
        //还有可能是备战区
        for (int r = 0; r < board->getBen(); ++r) {
            if (benchWidgets[r]==clickedWidget && !board->isCellEmpty(r)&& board->getUnitAt(r)->getOwner()==0) {
                // 选中成功！记录状态
                isDragging = true;
                dragFromPos = r;
                dragHero= board->getUnitAt(r);
                // std::cout<< "choose successfully, update the parameter"<<std::endl;
                // std::cout<<"parameter: "<< dragFromRow<<" , "<<dragFromCol<<" , "<<dragFromPos<<std::endl;
                benchWidgets[r]->setStyleSheet("background-color: #ffffcc; border: 3px solid yellow;");
                return;
            }
        }
    }
    QMainWindow::mousePressEvent(event);
}


// 鼠标松开事件：负责落子
void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDragging) {
        QWidget *releaseWidget = childAt(event->pos());
        if (releaseWidget) {
            // std::cout<<"catch release"<<std::endl;
            // 遍历board，看松开的位置是不是合法的空白格子
            bool isFind = false;
            for (int r = board->getRows()-1; r >= board->getRows()/2; --r) {
                for (int c = 0; c < board->getCols(); ++c) {
                    if (gridWidgets[r][c] == releaseWidget && board->isCellEmpty(r, c)) {
                        // std::cout<<"find in board"<<std::endl;
                        // 逻辑层移动棋子
                        board->placeUnitAt(r,c, dragHero);
                        board->removeUnitAt(dragFromRow,dragFromCol);
                        board->removeUnitAt(dragFromPos);
                        isFind = true;
                        break;
                    }
                }
                if (isFind) {break;}
            }

            //如果是bench，遍历备战区
            for (int r = 0; r < board->getBen(); ++r) {
                if (benchWidgets[r] == releaseWidget && board->isCellEmpty(r)) {
                    // std::cout<<"find in bench"<<std::endl;
                    //逻辑层移动棋子
                    board->placeUnitAt(r, dragHero);
                    board->removeUnitAt(dragFromRow,dragFromCol);
                    board->removeUnitAt(dragFromPos);
                    // std::cout<<"logic level move successfully"<<std::endl;
                    isFind = true;
                    break;
                }
            }
            // if (isFind) {
            // std::cout<<"find unit and have already moved successfully"<<std::endl;
            // }else {
            // std::cout<<"find nothing"<<std::endl;
            // }
        }
        //逻辑层移动完毕，刷新GUI界面显示
        updateBoardUI();
        //重置参数,清除高亮
        if (dragFromPos!=-1) {
            benchWidgets[dragFromPos]->setStyleSheet("background-color: white; border: 0.1px solid gray;");
        }
        if (dragFromRow!=-1 && dragFromCol!=-1) {
            gridWidgets[dragFromRow][dragFromCol]->setStyleSheet("background-color: white; border: 0.1px solid gray;");
        }
        isDragging = false;
        dragFromRow = -1;
        dragFromCol = -1;
        dragFromPos = -1;
        dragHero= nullptr;
        // std::cout<<"reset the parameter"<<std::endl;
    }
}


// 刷新棋盘显示
void GameWindow::updateBoardUI() {
    // 刷新棋盘
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            PieceWidget* w = gridWidgets[r][c];
            if (board->isCellEmpty(r, c)) {
                w->setUnit(nullptr);
            } else {
                w->setUnit(board->getUnitAt(r,c));
            }
        }
    }

    // 刷新备战区
    for (int i = 0; i < board->getBen(); ++i) {
        PieceWidget* w = benchWidgets[i];
        if (board->isCellEmpty(i)) {
            w->setUnit(nullptr);
        } else {
            w->setUnit(board->getUnitAt(i));
        }
    }

    std::cout<<"refresh successfully"<<std::endl;

}

// PieceWidget* GameWindow::getPieceWidget(const int row,const int col) {
//     if (row>=0 && row<board->getRows() && col>=0 && col<board->getCols()) {
//         std::cout<<"get piece in board"<<std::endl;
//         return gridWidgets[row][col];
//     }
//     std::cout<<"get nothing in board"<<std::endl;
//     return nullptr;
// }
//
// PieceWidget* GameWindow::getPieceWidget(const int pos) {
//     if (pos>=0 && pos<board->getBen()) {
//         std::cout<<"get piece in bench"<<std::endl;
//         return benchWidgets[pos];
//     }
//     std::cout<<"get nothing in bench"<<std::endl;
//     return nullptr;
// }