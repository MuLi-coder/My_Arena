#include "GameWindow.h"
#include <QGridLayout>
#include "../entity/Hero/Knight.h"
#include <QPixmap>
#include <QVBoxLayout>
#include "PieceWidget.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    gameManager = std::make_unique<GameManager>(8,8,8);
    connect(gameManager.get(),&GameManager::shouldUpdate,this,&GameWindow::updateBoardUI);
    // 状态记录参数的初始化
    isDragging = false;
    dragFromRow = -1;
    dragFromCol = -1;
    dragFromPos = -1;
    dragHero = nullptr;

    //---------------测试棋子------------
    gameManager->placeUnitAt(3,new Knight("test 1",3));
    gameManager->placeUnitAt(4,new Knight("test 2",4));
    gameManager->placeUnitAt(2,2,new Knight("test 3",5,1));
    //---------------------------------

    //--------------------------下面就是画图的部分-----------------------------//
    // 1. 设置窗口基础属性
    setWindowTitle("my_Arena");
    resize(600, 800); // 稍微调高一点窗口，给上下备战区留出足够空间

    // 2. 设置中央部件/
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
    boardPart->setFixedSize((CELL_SIZE+10)*gameManager->getCol(), (CELL_SIZE+20)*gameManager->getRow());
    //设置垂直布局和基本样式
    QGridLayout *boardLayout = new QGridLayout(boardPart);
    boardLayout->setContentsMargins(10, 10, 10, 10);
    boardLayout->setSpacing(4);

    for (int r = 0; r < gameManager->getRow(); ++r) {
        // Data:每行的格子容器vector，临时容器，负责最后塞到成员中
        std::vector<PieceWidget*> rowLabels;
        for (int c = 0; c < gameManager->getCol(); ++c) {
            //把网格虚拟布局的每个格子的位置放一个控件，显示出格子的形态
            QWidget* cell = new QWidget(this);
            cell->setFixedSize(CELL_SIZE, CELL_SIZE+8);
            //样式敌我分离
            if (r<gameManager->getRow()/2) {
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

    for (int c = 0; c < gameManager->getBen(); ++c) {
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
    controlButtonPart->setFixedSize(CELL_SIZE*gameManager->getBen(), CELL_SIZE+8);
    QHBoxLayout* controlButtonLayout = new QHBoxLayout(controlButtonPart);
    // 第一个开始战斗按钮
    QPushButton* combatButton = new QPushButton("Combat", controlButtonPart); // 直接传入文字和父控件
    combatButton->setFixedSize(70, 40);
    combatButton->setStyleSheet("color:red;border: 1px solid gray; background:white ;");
    connect(combatButton, &QPushButton::clicked, this, [=]() {
        gameManager->changeStateTo(GameState::Combat);
    });

    // 第二个返回备战按钮
    QPushButton* prepareButton = new QPushButton("Prepare", controlButtonPart);
    prepareButton->setFixedSize(70, 40);
    prepareButton->setStyleSheet("color:red; border: 1px solid gray; background: white;");
    connect(prepareButton, &QPushButton::clicked, this, [=]() {
        gameManager->changeStateTo(GameState::Prepare);
    });

    //第三个暂停按钮
    QPushButton* pauseButton = new QPushButton("Pause", controlButtonPart);
    pauseButton->setFixedSize(70, 40);
    pauseButton->setStyleSheet("background:#ed897b;border: 1px solid gray; ");
    connect(pauseButton, &QPushButton::clicked, this, [=]() {
       if (gameManager->getCurrentState()==GameState::Combat) {
           gameManager->timer_stop();
       }
    });
    //添加到水平布局中
    controlButtonLayout->addWidget(prepareButton);
    controlButtonLayout->addWidget(combatButton);
    controlButtonLayout->addWidget(pauseButton);
    controlButtonLayout->setAlignment(Qt::AlignCenter);
    //将水平布局添加到窗口布局中
    windowVLayout->addWidget(controlButtonPart,1,Qt::AlignCenter);
    //------------------------画图部分结束-----------------------------//

    // 刷新棋盘显示
    this->updateBoardUI();
}


// 鼠标按下事件：负责“选中”棋子
void GameWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && gameManager->getCurrentState()==GameState::Prepare) {
        // 获取鼠标点击位置的子控件
        QWidget *clickedWidget = childAt(event->pos());
        if (!clickedWidget) return;
        for (int r = 0; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                if (gridWidgets[r][c] == clickedWidget && !gameManager->isEmpty(r,c) && gameManager->getUnitAt(r,c)->getOwner()==0) {
                    isDragging =true;
                    dragHero = gameManager->getUnitAt(r,c);
                    dragFromRow = r;
                    dragFromCol = c;
                    gridWidgets[r][c]->setStyleSheet("background-color: #ffffcc; border: 3px solid yellow;");
                    return;
                }
            }
        }
        //还有可能是备战区
        for (int k = 0; k < gameManager->getBen(); ++k) {
            if (benchWidgets[k]==clickedWidget && !gameManager->isEmpty(k) && gameManager->getUnitAt(k)->getOwner()==0) {
                // 选中成功！清除原位状态，记录状态
                isDragging = true;
                dragHero = gameManager->getUnitAt(k);
                dragFromPos = k;
                benchWidgets[k]->setStyleSheet("background-color: #ffffcc; border: 3px solid yellow;");
                return;
                // std::cout<<"parameter: "<< dragFromRow<<" , "<<dragFromCol<<" , "<<dragFromPos<<std::endl;
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
            bool isFind = false;
            for (int r = gameManager->getRow()-1; r >= gameManager->getRow()/2; --r) {
                for (int c = 0; c < gameManager->getCol(); ++c) {
                    if (gridWidgets[r][c] == releaseWidget && gameManager->isEmpty(r,c)) {
                        //找到空格并落子
                        gameManager->placeUnitAt(r,c,dragHero);
                        gameManager->removeUnitAt(dragFromRow,dragFromCol);
                        gameManager->removeUnitAt(dragFromPos);
                        isFind = true;
                        break;
                    }
                }
                if (isFind) {break;}
            }
            //如果是bench，遍历备战区
            for (int k = 0; k < gameManager->getBen(); ++k) {
                if (benchWidgets[k] == releaseWidget && gameManager->isEmpty(k)) {
                    //找到空格并且落子
                    gameManager->placeUnitAt(k,dragHero);
                    gameManager->removeUnitAt(dragFromRow,dragFromCol);
                    gameManager->removeUnitAt(dragFromPos);
                    isFind = true;
                    break;
                }
            }
        }
        //移动完毕，刷新GUI界面显示，重置参数
        updateBoardUI();
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
    GameState state = gameManager->getCurrentState();
    // 备战时刷新逻辑
    if (state==GameState::Prepare) {
        for (int r = 0; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                PieceWidget* w = gridWidgets[r][c];
                w->setUnit(gameManager->getUnitAt(r,c));
            }
        }
        for (int k = 0; k < gameManager->getBen(); ++k) {
            PieceWidget* w = benchWidgets[k];
            w->setUnit(gameManager->getUnitAt(k));
        }

    }
    //战斗时刷新逻辑
    else if (state==GameState::Combat) {
        //先全部置空
        for (int r=0; r<gameManager->getRow(); ++r) {
            for (int c=0; c<gameManager->getCol(); ++c) {
                PieceWidget* w = gridWidgets[r][c];
                w->setUnit(nullptr);
            }
        }
        //然后根据有的棋子渲染
        for (int r = 0; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                Unit* unit = gameManager->getUnitAt(r,c);
                if (unit) {
                    int x = unit->getX();
                    int y = unit->getY();
                    this->getPieceWidget(x,y)->setUnit(unit);
                }
            }
        }
    }
    //
}

PieceWidget* GameWindow::getPieceWidget(const int r,const int c) {
    if (r>=0 && r<gameManager->getRow() && c>=0 && c< gameManager->getCol()) {
        return gridWidgets[r][c];
    }
    return nullptr;
}

PieceWidget* GameWindow::getPieceWidget(const int pos) {
    if (pos>=0 && pos<gameManager->getBen()) {
        return benchWidgets[pos];
    }
    return nullptr;
}