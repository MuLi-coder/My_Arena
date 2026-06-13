#include "GameWindow.h"
#include <QGridLayout>
#include <QVBoxLayout>


GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //创建管理者实例
    gameManager = std::make_unique<GameManager>(8,8,8);
    //将下游信号和刷新函数绑定
    connect(gameManager.get(),&GameManager::shouldUpdate,this,&GameWindow::updateBoardUI);

    // 状态记录参数的初始化
    isDragging = false;
    dragFromRow = -1;
    dragFromCol = -1;
    dragFromPos = -1;
    dragHero = nullptr;

    // //---------------测试棋子------------
    gameManager->placeUnitAtGrid(2,2,new Knight("Knight 1",1));
    gameManager->placeUnitAtGrid(3,3,new Knight("Knight 2",1));
    gameManager->placeUnitAtGrid(2,5,new Mage("Mage 1",1));
    // //---------------------------------
    gameManager->refreshShop();

    //--------------------------下面就是画图的部分的初始化-----------------------------//
    // 1. 设置窗口基础属性
    setWindowTitle("my_Arena");
    resize(600, 800); // 稍微调高一点窗口，给上下备战区留出足够空间

    // 2. 设置中央部件，并把这个窗口主控件设置为中央控件（强制规定）
    QWidget* windowWidget = new QWidget(this);
    setCentralWidget(windowWidget);
    //为窗口大控件设置布局方式
    QHBoxLayout* windowLayout = new QHBoxLayout(windowWidget);
    windowLayout->setContentsMargins(20, 20, 20, 20);
    windowLayout->setSpacing(20);


    //------------------------左半边两部分，棋盘和备战区-------------------------//
    QVBoxLayout *LeftVLayout = new QVBoxLayout(); // 改用垂直布局
    LeftVLayout->setSpacing(20); // 设置上下两部分之间的间距（空气感）
    LeftVLayout->setContentsMargins(20, 20, 20, 20); // 设置整体边距

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
    LeftVLayout->addWidget(boardPart,0,Qt::AlignCenter);

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
    LeftVLayout->addWidget(benchPart, 0,Qt::AlignCenter);
    //-------------------------左半边画完，放入最大水平布局中-----------------------//
    windowLayout->addLayout(LeftVLayout);
    //------------------------下面画右半边两部分，游戏状态面板，控制按钮，商店-------------------------------------//
    QVBoxLayout* rightVLayout = new QVBoxLayout();
    rightVLayout->setContentsMargins(20, 20, 20, 20);
    rightVLayout->setSpacing(20);

    //-------------第一部分，控制面板---------------//


    //-----------第二部分，画下方的开始结束按钮---------
    //大框控件
    QWidget* controlButtonPart = new QWidget(this);
    controlButtonPart->setStyleSheet("border:1px solid gray;background:white");
    controlButtonPart->setFixedSize(CELL_SIZE*gameManager->getBen(), CELL_SIZE+8);
    QHBoxLayout* controlButtonLayout = new QHBoxLayout(controlButtonPart);
    // 第一个开始战斗按钮
    QPushButton* combatButton = new QPushButton("Combat", controlButtonPart); // 直接传入文字和父控件
    combatButton->setFixedSize(70, 40);
    combatButton->setStyleSheet("color:red;border: 1px solid gray; background:white ;");
    //将按钮绑定需要激发的函数：切换战斗阶段
    connect(combatButton, &QPushButton::clicked, this, [=]() {
        gameManager->changeStateTo(GameState::Combat);
    });

    // 第二个返回备战按钮
    QPushButton* prepareButton = new QPushButton("Prepare", controlButtonPart);
    prepareButton->setFixedSize(70, 40);
    prepareButton->setStyleSheet("color:red; border: 1px solid gray; background: white;");
    //将按钮绑定需要激发的函数：准换到备战阶段
    connect(prepareButton, &QPushButton::clicked, this, [=]() {
        gameManager->changeStateTo(GameState::Prepare);
    });

    //第三个暂停按钮
    QPushButton* pauseButton = new QPushButton("Pause", controlButtonPart);
    pauseButton->setFixedSize(70, 40);
    pauseButton->setStyleSheet("background:#ed897b;border: 1px solid gray; ");
    //绑定激发的函数：暂停秒表
    connect(pauseButton, &QPushButton::clicked, this, [=]() {
       if (gameManager->getCurrentState()==GameState::Combat) {
           if (gameManager->isTimerActive()) {
               gameManager->timerStop();
           }else {
               gameManager->timerStart();
           }
       }
    });
    //添加到水平布局中
    controlButtonLayout->addWidget(prepareButton);
    controlButtonLayout->addWidget(combatButton);
    controlButtonLayout->addWidget(pauseButton);
    controlButtonLayout->setAlignment(Qt::AlignCenter);
    //将水平布局添加到窗口布局中
    rightVLayout->addWidget(controlButtonPart,1,Qt::AlignCenter);
    //---------------第三部分，商店----------------------//
    QWidget* shopPart = new QWidget(this);
    shopPart->setStyleSheet("border:1px solid gray;background:white");
    shopPart->setFixedSize(CELL_SIZE*gameManager->getBen(), 3*CELL_SIZE+8);
    QVBoxLayout* shopLayout = new QVBoxLayout(shopPart);
    shopLayout->setContentsMargins(0, 0, 0, 0);
    shopLayout->setSpacing(5);
    //-----上方文字标签和刷新按钮-----
    QHBoxLayout* upperShopLayout = new QHBoxLayout(shopPart);
    upperShopLayout->setContentsMargins(0, 0, 0, 0);
    upperShopLayout->setSpacing(20);
    //文字标签
    QLabel* shopLabel = new QLabel("SHOP");
    shopLabel->setStyleSheet("color:black;border:1px solid gray;background:white");
    shopLabel->setFixedSize(CELL_SIZE, CELL_SIZE-10);
    shopLabel->setAlignment(Qt::AlignCenter);
    //刷新按钮
    QPushButton* refreshButton = new QPushButton("refresh",shopPart);
    refreshButton->setFixedSize(70, 40);
    refreshButton->setStyleSheet("background:#ed897b;border: 1px solid gray; ");
    //绑定激发的函数
    connect(refreshButton, &QPushButton::clicked, this, [=]() {
        gameManager->refreshShop();
        updateBoardUI();
    });
    upperShopLayout->addWidget(shopLabel,1,Qt::AlignCenter);
    upperShopLayout->addWidget(refreshButton,1,Qt::AlignCenter);

    //-----下方棋子区-----
    QWidget* shopBenchPart = new QWidget(shopPart);
    shopBenchPart->setStyleSheet("background:white;border:1px solid ;");
    // 设置水平布局
    QHBoxLayout *shopBenchLayout = new QHBoxLayout(shopBenchPart);
    shopBenchLayout->setSpacing(5);

    for (int c = 0; c < 5; ++c) {
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
        shopBenchLayout->addWidget(cell);

        // Data: 记录
        shopBenchWidgets.push_back(pieceWidget);
    }
    shopBenchLayout->setAlignment(Qt::AlignHCenter);

    //加入shopPart
    shopLayout->addLayout(upperShopLayout);
    shopLayout->addWidget(shopBenchPart, 0,Qt::AlignCenter);
    shopLayout->setAlignment(Qt::AlignCenter);
    //商店画完，加入右侧垂直布局
    rightVLayout->addWidget(shopPart,1,Qt::AlignCenter);

    //---------------右半边画完，加入大布局-------------//
    windowLayout->addLayout(rightVLayout);
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
                if (gridWidgets[r][c] == clickedWidget && !gameManager->isCellEmptyGrid(r,c) && gameManager->getUnitAtGrid(r,c)->getOwner()==0) {
                    isDragging =true;
                    dragHero = gameManager->getUnitAtGrid(r,c);
                    dragFromRow = r;
                    dragFromCol = c;
                    gridWidgets[r][c]->setStyleSheet("background-color: #ffffcc; border: 3px solid yellow;");
                    return;
                }
            }
        }
        //还有可能是备战区
        for (int k = 0; k < gameManager->getBen(); ++k) {
            if (benchWidgets[k]==clickedWidget && !gameManager->isCellEmptyBench(k) && gameManager->getUnitAtBench(k)->getOwner()==0) {
                // 选中成功！清除原位状态，记录状态
                isDragging = true;
                dragHero = gameManager->getUnitAtBench(k);
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
        if (!releaseWidget) return;
        bool isFind = false;
        for (int r = gameManager->getRow()-1; r >= gameManager->getRow()/2; --r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                if (gridWidgets[r][c] == releaseWidget && gameManager->isCellEmptyGrid(r,c)) {
                    //找到对应空格并落子
                    gameManager->placeUnitAtGrid(r,c,dragHero);
                    gameManager->removeUnitAtGrid(dragFromRow,dragFromCol);
                    gameManager->removeUnitAtBench(dragFromPos);
                    isFind = true;
                    break;
                }
            }
            if (isFind) {break;}
        }
        //如果是bench，遍历备战区
        for (int k = 0; k < gameManager->getBen(); ++k) {
            if (benchWidgets[k] == releaseWidget && gameManager->isCellEmptyBench(k)) {
                //找到空格并且落子
                gameManager->placeUnitAtBench(k,dragHero);
                gameManager->removeUnitAtGrid(dragFromRow,dragFromCol);
                gameManager->removeUnitAtBench(dragFromPos);
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
}

//双击购买逻辑
void GameWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && gameManager->getCurrentState()==GameState::Prepare) {
        QWidget *doubleClickWidget = childAt(event->pos());
        if (!doubleClickWidget) return;
        for (int s=0;s<5;++s) {
            if (doubleClickWidget==shopBenchWidgets[s] && !gameManager->isCellEmptyShop(s)) {
                shopHero = gameManager->getUnitAtShop(s);
                bool isBoughtSucceed=false;
                for (int k=0;k<gameManager->getBen();++k) {
                    if (gameManager->isCellEmptyBench(k)) {
                        gameManager->placeUnitAtBench(k,shopHero);
                        isBoughtSucceed = true;
                        break;
                    }
                }
                if (isBoughtSucceed) {
                    gameManager->removeUnitAtShop(s);
                }
            }
        }
    }
    QWidget::mouseDoubleClickEvent(event);
}


// 刷新棋盘显示
void GameWindow::updateBoardUI() {
    GameState state = gameManager->getCurrentState();
    // 备战时刷新逻辑,严格根据棋子数据位置定位，board层数据垂直映射
    if (state==GameState::Prepare) {
        for (int r = 0; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                PieceWidget* w = gridWidgets[r][c];
                w->setUnit(gameManager->getUnitAtGrid(r,c));
            }
        }
        for (int k = 0; k < gameManager->getBen(); ++k) {
            PieceWidget* w = benchWidgets[k];
            w->setUnit(gameManager->getUnitAtBench(k));
        }
        for (int s=0; s < 5; ++s) {
            PieceWidget* w = shopBenchWidgets[s];
            w->setUnit(gameManager->getUnitAtShop(s));
        }
    }

    //战斗时刷新逻辑，更换更新逻辑，数据源来自ComBoard
    else if (state==GameState::Combat) {
        for (int r = 0; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                PieceWidget* w = gridWidgets[r][c];
                w->setUnit(gameManager->getUnitAtGrid(r,c));
            }
        }
        for (int k = 0; k < gameManager->getBen(); ++k) {
            PieceWidget* w = benchWidgets[k];
            w->setUnit(gameManager->getUnitAtBench(k));
        }
        for (int s=0; s < 5; ++s) {
            PieceWidget* w = shopBenchWidgets[s];
            w->setUnit(gameManager->getUnitAtShop(s));
        }
    }
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