#include "GameWindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "../entity/Equipment/Sword.h"
#include "../entity/Equipment/BlueCrystal.h"
#include "../entity/Equipment/ChainMail.h"
#include "../entity/Equipment/RapidGloves.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent) {
    //创建管理者实例
    gameManager = std::make_unique<GameManager>(8,8,8);
    //将下游信号和各自的槽函数绑定
    connect(gameManager.get(),&GameManager::shouldUpdate,this,&GameWindow::updateBoardUI);
    connect(gameManager.get(),&GameManager::enterResolve,this,&GameWindow::resolveMessageShow);
    connect(gameManager.get(),&GameManager::failToCombat,this,&GameWindow::failToCombatMessage);
    connect(gameManager.get(),&GameManager::traitActivate,this,&GameWindow::traitActivateShow);
    // 状态记录参数的初始化
    isDragging = false;
    isEquipping = false;
    dragFromRow = -1;
    dragFromCol = -1;
    dragFromPos = -1;
    dragFromEquip = -1;
    dragEquipment = nullptr;
    shopHero = nullptr;
    dragHero = nullptr;

    //--------------------------下面剩下的就是画图的部分的初始化-----------------------------//

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
    //使用自己定义的控件，用于更新数据
    paraPart = new ParaWidget(this);
    paraPart->setPara(gameManager->getPlayer());
    rightVLayout->addWidget(paraPart);

    //-----------第二部分，画下方的开始结束按钮---------
    //大框控件
    QWidget* controlButtonPart = new QWidget(this);
    controlButtonPart->setStyleSheet("border:1px solid gray;background:white");
    controlButtonPart->setFixedSize(CELL_SIZE*gameManager->getBen(), CELL_SIZE+8);
    QHBoxLayout* controlButtonLayout = new QHBoxLayout(controlButtonPart);
    controlButtonLayout->setContentsMargins(0, 0, 0, 0);
    controlButtonLayout->setSpacing(2);

    //升级按钮
    QPushButton* levelUpButton = new QPushButton("LevelUp(5)", controlButtonPart);
    levelUpButton->setFixedSize(70, 40);
    levelUpButton->setStyleSheet("color:red; border: 1px solid gray; background: white;");
    //将按钮绑定需要激发的函数：准换到备战阶段
    connect(levelUpButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Prepare) {
            if (gameManager->getPlayerMoney()<5) {
                QMessageBox::information(this, "失败", "金币不足");
                return;
            }
            if (gameManager->getPlayerLevel()==7) {
                QMessageBox::information(this,"失败","已 7 级满级");
                return;
            }
            gameManager->levelUp();
            gameManager->changePlayerMoney(-5);
            updateBoardUI();
        }
    });
    // 第一个返回备战按钮
    QPushButton* prepareButton = new QPushButton("Prepare", controlButtonPart);
    prepareButton->setFixedSize(70, 40);
    prepareButton->setStyleSheet("color:red; border: 1px solid gray; background: white;");
    //将按钮绑定需要激发的函数：准换到备战阶段
    connect(prepareButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Combat) {
            gameManager->changeStateTo(GameState::Prepare);
        }
    });
    // 第二个开始战斗按钮
    QPushButton* combatButton = new QPushButton("Combat", controlButtonPart); // 直接传入文字和父控件
    combatButton->setFixedSize(70, 40);
    combatButton->setStyleSheet("color:red;border: 1px solid gray; background:white ;");
    //将按钮绑定需要激发的函数：切换战斗阶段
    connect(combatButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Prepare) {
            gameManager->changeStateTo(GameState::Combat);
        }
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
    //第四个开始游戏按钮
    QPushButton* startButton = new QPushButton("START", controlButtonPart);
    startButton->setFixedSize(70, 40);
    startButton->setStyleSheet("color:white; border: 1px solid gray; background: purple;");
    //将按钮绑定需要激发的函数：准换到备战阶段
    connect(startButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Ready) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText(QString(
                "----------游戏开始------------\n"
                "你初始有金币:  %1\n"
                "你一共有 %2 次刷新商店的机会"
                )
                .arg(gameManager->getPlayerMoney())
                .arg(gameManager->getPlayerShopRefreshTimes())
                );
            msgBox.exec();
            gameManager->refreshShop();
            gameManager->changeStateTo(GameState::Prepare);
        }
    });

    //第五个存档按钮
    QPushButton* saveButton = new QPushButton("Save", controlButtonPart);
    saveButton->setFixedSize(70, 40);
    saveButton->setStyleSheet("color:white; border: 1px solid gray; background: #4a90d9;");
    connect(saveButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Prepare) {
            int ret = QMessageBox::question(
                this,
                "存档",
                "确定要保存当前游戏进度吗？",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
            );
            if (ret == QMessageBox::Yes) {
                QString path = QFileDialog::getSaveFileName(this, "选择存档位置", "", "存档文件 (*.sav)");
                if (!path.isEmpty()) {
                    gameManager->saveGame(path);
                    QMessageBox::information(this, "成功", "游戏已保存");
                }
            }
        } else {
            QMessageBox::information(this, "提示", "只能在准备阶段存档");
        }
    });

    //第六个读档按钮
    QPushButton* loadButton = new QPushButton("Load", controlButtonPart);
    loadButton->setFixedSize(70, 40);
    loadButton->setStyleSheet("color:white; border: 1px solid gray; background: #7b68ee;");
    connect(loadButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Ready || gameManager->getCurrentState()==GameState::Prepare) {
            QString path = QFileDialog::getOpenFileName(this, "选择存档文件", "", "存档文件 (*.sav)");
            if (!path.isEmpty()) {
                gameManager->loadGame(path);
                QMessageBox::information(this, "成功", "存档已加载");
                updateBoardUI();
            }
        } else {
            QMessageBox::information(this, "提示", "只能在准备阶段或游戏未开始时读档");
        }
    });

    //将六个按钮添加到水平布局中
    controlButtonLayout->addWidget(levelUpButton);
    controlButtonLayout->addWidget(prepareButton);
    controlButtonLayout->addWidget(combatButton);
    controlButtonLayout->addWidget(pauseButton);
    controlButtonLayout->addWidget(startButton);
    controlButtonLayout->addWidget(saveButton);
    controlButtonLayout->addWidget(loadButton);
    controlButtonLayout->setAlignment(Qt::AlignCenter);
    //将水平布局添加到窗口布局中
    rightVLayout->addWidget(controlButtonPart,1,Qt::AlignCenter);

    //----------------第三部分：主动装备----------------//
    QWidget* equipPart = new QWidget();
    equipPart->setStyleSheet("background:white;border:1px solid ;");
    // 设置水平布局
    QHBoxLayout *equipLayout = new QHBoxLayout(equipPart);
    equipLayout->setSpacing(5);
    for (int c = 0; c < 5; ++c) {
        // 创建单个格子控件
        QWidget* cell = new QWidget();
        cell->setFixedSize(CELL_SIZE, CELL_SIZE);
        cell->setStyleSheet("border: 1px solid gray; background: #fff7f0;");
        //给每个格子创建垂直布局
        QVBoxLayout* cellLayout = new QVBoxLayout(cell);
        cellLayout->setContentsMargins(0, 0, 0, 0);
        // 创建棋子控件，并把棋子控件放入格子的布局
        EquipWidget* equipWidget = new EquipWidget(cell);
        cellLayout->addWidget(equipWidget, 0, Qt::AlignCenter);
        // 把整个格子加入水平布局
        equipLayout->addWidget(cell);

        // Data: 记录
        equipWidgets.push_back(equipWidget);
    }
    equipLayout->setAlignment(Qt::AlignHCenter);

    rightVLayout->addWidget(equipPart);


    //---------------第四部分，商店----------------------//
    QWidget* shopPart = new QWidget(this);
    shopPart->setStyleSheet("background:white");
    shopPart->setFixedSize(CELL_SIZE*gameManager->getBen(), 3*CELL_SIZE+8);
    QVBoxLayout* shopLayout = new QVBoxLayout(shopPart);
    shopLayout->setContentsMargins(0, 0, 0, 0);
    shopLayout->setSpacing(5);
    //-----上方文字标签和刷新按钮-----
    QWidget* shopUpPart = new QWidget(shopPart);
    shopUpPart->setFixedSize(CELL_SIZE*gameManager->getBen(), CELL_SIZE);
    shopUpPart->setStyleSheet("border:0.3px");
    QHBoxLayout* upperShopLayout = new QHBoxLayout(shopUpPart);
    upperShopLayout->setContentsMargins(0, 0, 0, 0);
    upperShopLayout->setSpacing(20);

    //文字标签
    QLabel* shopLabel = new QLabel("SHOP");
    shopLabel->setStyleSheet("color:black;border:1px solid gray;background:white");
    shopLabel->setFixedSize(CELL_SIZE, CELL_SIZE-10);
    shopLabel->setAlignment(Qt::AlignCenter);
    //刷新按钮
    QPushButton* refreshButton = new QPushButton(shopPart);
    refreshButton->setText(QString("refresh(2)"));
    refreshButton->setFixedSize(70, 40);
    refreshButton->setStyleSheet("color:red;border:1px solid gray;background:white;");
    connect(refreshButton, &QPushButton::clicked, this, [=]() {
        if (gameManager->getCurrentState()==GameState::Prepare) {
            int ret = QMessageBox::question(
                    this,
                    "确认操作",
                    "确定要刷新商店吗",
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                );

            if (ret == QMessageBox::Yes) {
                if (gameManager->getPlayerShopRefreshTimes()==0) {
                    QMessageBox::information(this,"操作失败！",QString("刷新次数已耗尽"));
                    return;
                };
                if (gameManager->getPlayerMoney()<2) {
                    QMessageBox::information(this,"失败","金币不足");
                    return;
                }
                gameManager->refreshShop();
                gameManager->changeShopRefreshTimes(-1);
                gameManager->changePlayerMoney(-2);
                updateBoardUI();
                QMessageBox::information(this,"操作成功！",QString("商店刷新,剩余刷新次数：%1").arg(gameManager->getPlayerShopRefreshTimes()));
            }
        }
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
    shopLayout->addWidget(shopUpPart);
    shopLayout->addWidget(shopBenchPart, 0,Qt::AlignCenter);
    shopLayout->setAlignment(Qt::AlignCenter);
    //商店画完，加入右侧垂直布局
    rightVLayout->addWidget(shopPart,1,Qt::AlignCenter);

    //---------------右半边画完，加入大布局-------------//
    windowLayout->addLayout(rightVLayout);
    //------------------------画图部分结束-----------------------------//

}


// 鼠标按下事件：负责”选中”棋子
void GameWindow::mousePressEvent(QMouseEvent *event) {
    //右键脱下装备
    if (event->button() == Qt::RightButton && gameManager->getCurrentState()==GameState::Prepare) {
        QWidget *clickedWidget = childAt(event->pos());
        if (!clickedWidget) return;

        Unit* targetUnit = nullptr;

        //检查是否点击了我方棋盘上的棋子
        for (int r = gameManager->getRow()/2; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                if (gridWidgets[r][c] == clickedWidget && !gameManager->isCellEmptyGrid(r,c) && gameManager->getUnitAtGrid(r,c)->getOwner()==0) {
                    targetUnit = gameManager->getUnitAtGrid(r,c);
                    break;
                }
            }
            if (targetUnit) break;
        }

        //检查是否点击了备战区的棋子
        if (!targetUnit) {
            for (int k = 0; k < gameManager->getBen(); ++k) {
                if (benchWidgets[k] == clickedWidget && !gameManager->isCellEmptyBench(k) && gameManager->getUnitAtBench(k)->getOwner()==0) {
                    targetUnit = gameManager->getUnitAtBench(k);
                    break;
                }
            }
        }

        //如果找到了有装备的棋子，弹窗询问是否脱下
        if (targetUnit && targetUnit->isWearingEquipment()) {
            int ret = QMessageBox::question(
                this,
                "脱下装备",
                QString("确定要脱下 %1 的装备吗？").arg(targetUnit->getName()),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
            );

            if (ret == QMessageBox::Yes) {
                //检查装备区是否有空位
                bool hasEmptySlot = false;
                for (int k = 0; k < 5; ++k) {
                    if (gameManager->isEquipmentEmpty(k)) {
                        hasEmptySlot = true;
                        Equipment* equip = targetUnit->takeOffEquipment(true);
                        gameManager->placeEquipmentAt(k, equip);
                        targetUnit->selfRefresh();
                        QMessageBox::information(this, "成功", "装备已脱下");
                        break;
                    }
                }
                if (!hasEmptySlot) {
                    QMessageBox::information(this, "失败", "装备区已满，无法脱下装备");
                }
            }
            updateBoardUI();
        }
    }
    //左键选中棋子
    else if (event->button() == Qt::LeftButton && gameManager->getCurrentState()==GameState::Prepare) {
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
            }
        }
        //还可能是装备区
        std::cout<<clickedWidget<<std::endl;
        for (int s = 0;s<5;++s) {
            if (equipWidgets[s]== clickedWidget && !gameManager->isEquipmentEmpty(s)) {
                isEquipping = true;
                dragEquipment = gameManager->getEquipmentAt(s);
                dragFromEquip = s;
                equipWidgets[s]->setStyleSheet("background-color: #ffffcc; border: 3px solid yellow;");
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
        if (!releaseWidget) return;
        bool isFind = false;
        for (int r = gameManager->getRow()-1; r >= gameManager->getRow()/2; --r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                if (gridWidgets[r][c] == releaseWidget && gameManager->isCellEmptyGrid(r,c)) {
                    if ( (dragFromPos!=-1 && gameManager->selfUnitCount()<gameManager->getMaxUnit())|| (dragFromRow!=-1 && dragFromCol != -1) ) {
                        //找到对应空格并落子
                        gameManager->placeUnitAtGrid(r,c,dragHero);
                        gameManager->removeUnitAtGrid(dragFromRow,dragFromCol);
                        gameManager->removeUnitAtBench(dragFromPos);
                        isFind = true;
                        break;
                    }
                    QMessageBox::information(this,"操作失败","人数已达上限");
                }
            }
            if (isFind) {break;}
        }
        //如果是bench，遍历备战区
        if (!isFind) {
            for (int k = 0; k < gameManager->getBen(); ++k) {
                if (benchWidgets[k] == releaseWidget && gameManager->isCellEmptyBench(k)) {
                    //找到空格并且落子
                    gameManager->placeUnitAtBench(k,dragHero);
                    gameManager->removeUnitAtGrid(dragFromRow,dragFromCol);
                    gameManager->removeUnitAtBench(dragFromPos);
                    break;
                }
            }
        }
    }

    //还有可能是穿装备
    else if (event->button()== Qt::LeftButton && isEquipping) {
        QWidget *releaseWidget = childAt(event->pos());
        if (!releaseWidget) return;
        //找到松手位置
        bool isFind = false;
        for (int r = gameManager->getRow()-1; r >= gameManager->getRow()/2; --r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                if (gridWidgets[r][c] == releaseWidget && !gameManager->isCellEmptyGrid(r,c)&& !gameManager->getUnitAtGrid(r,c)->isWearingEquipment()) {
                    gameManager->getUnitAtGrid(r,c)->putOnEquipment(dragEquipment);
                    gameManager->getUnitAtGrid(r,c)->selfRefresh();
                    gameManager->removeEquipmentAt(dragFromEquip);
                    isFind = true;
                    break;
                }
            }
            if (isFind) {break;}
        }
        //如果是bench，遍历备战区
        if (!isFind) {
            for (int k = 0; k < gameManager->getBen(); ++k) {
                if (benchWidgets[k] == releaseWidget && !gameManager->isCellEmptyBench(k) && !gameManager->getUnitAtBench(k)->isWearingEquipment()) {
                    gameManager->getUnitAtBench(k)->putOnEquipment(dragEquipment);
                    gameManager->getUnitAtBench(k)->selfRefresh();
                    gameManager->removeEquipmentAt(dragFromEquip);
                    break;
                }
            }
        }

    }

    //移动完毕，刷新GUI界面显示，重置参数
    updateBoardUI();
    isDragging = false;
    isEquipping = false;
    dragFromRow = -1;
    dragFromCol = -1;
    dragFromPos = -1;
    dragFromEquip = -1;
    shopHero = nullptr;
    dragHero= nullptr;
    dragEquipment= nullptr;
}

//双击购买逻辑
void GameWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && gameManager->getCurrentState()==GameState::Prepare) {
        QWidget *doubleClickWidget = childAt(event->pos());
        if (!doubleClickWidget) return;
        for (int s=0;s<5;++s) {
            if (doubleClickWidget==shopBenchWidgets[s] && !gameManager->isCellEmptyShop(s)) {
                shopHero = gameManager->getUnitAtShop(s);
                //弹窗提示
                int ret = QMessageBox::question(
                    this,
                    "确认操作",
                    QString("确定要购买该英雄吗?售价：%1 金币").arg(shopHero->getCost()),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                );

                if (ret == QMessageBox::Yes){
                    for (int k=0;k<gameManager->getBen();++k) {
                        if (gameManager->isCellEmptyBench(k)) {
                            if (gameManager->getPlayerMoney() < gameManager->getUnitAtShop(s)->getCost()) {
                                QMessageBox::information(this,"失败","金币不足");
                                break;
                            }
                            gameManager->placeUnitAtBench(k,shopHero);
                            gameManager->changePlayerMoney((-1)*gameManager->getUnitAtShop(s)->getCost());
                            gameManager->removeUnitAtShop(s);
                            //实现购买之后，要有自动合并升级的功能
                            gameManager->autoMergeToBench(k);
                            break;
                        }
                        if (k==gameManager->getBen()-1) {
                            QMessageBox::information(this,"失败","备战区已满");
                            break;
                        }
                    }
                }
            }
        }
    }
    updateBoardUI();
    QWidget::mouseDoubleClickEvent(event);
}


// 刷新棋盘显示
void GameWindow::updateBoardUI() {
    GameState state = gameManager->getCurrentState();

    if (state ==GameState::Ready) {
        for (int r = 0; r < gameManager->getRow(); ++r) {
            for (int c = 0; c < gameManager->getCol(); ++c) {
                PieceWidget* w = gridWidgets[r][c];
                w->setUnit(nullptr);
            }
        }
        for (int k =0; k < gameManager->getBen(); ++k) {
            PieceWidget* w = benchWidgets[k];
            w->setUnit(nullptr);
        }
        for (int r = 0; r < 5; ++r) {
            PieceWidget* w = shopBenchWidgets[r];
            w->setUnit(nullptr);
        }
    }

    else if (state==GameState::Prepare || state==GameState::Resolve) {
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
    }
    //刷新商店
    for (int s=0; s < 5; ++s) {
        PieceWidget* w = shopBenchWidgets[s];
        w->setUnit(gameManager->getUnitAtShop(s));
    }
    //刷新面板
    paraPart->setPara(gameManager->getPlayer());
    //刷新装备
    for (int r = 0; r < 5; ++r) {
        EquipWidget* e = equipWidgets[r];
        e->setEquip(gameManager->getEquipmentAt(r));
    }
}


void GameWindow::resolveMessageShow(ComResult result,const int hp) {
    switch (result) {
        case ComResult::Success:
            QMessageBox::information(this,"结算",
                "-----SUCCESS-----\n"
                "恭喜您，战斗获得了胜利\n"
                "奖励 20 金币, 积分+10\n"
                "同时，您的商店刷新次数已被重置为 3 次\n"
                "继续乘胜追击吧！"
                );
            break;
        case ComResult::Failure:
            if (hp!=0) {
                QMessageBox::information(this,"结算",
                    "-----FAILURE-----\n"
                    "很遗憾，没能乘胜追击\n"
                    "补贴 10 金币, hp-20\n"
                    "虽然如此，您的商店刷新次数也已被重置为 3 次\n"
                    "再接再厉！"
                    );
                break;
            }else {
                QMessageBox::information(this,"GAME OVER",
                    QString(
                        "GAME OVER\n"
                        "您的血量已经清零，漫漫征途就此止步\n"
                        "本次积分： %1").arg(gameManager->getPlayerScore())
                        );
                break;
            }

        case ComResult::Draw:
            QMessageBox::information(this,"结算",
                "-----DRAW-----\n"
                "这是一场势均力敌的战斗\n"
                "奖励 15 金币, 积分+5, hp-10\n"
                "您的商店刷新次数也已被重置为 3 次\n"
                "调整阵容，一鼓作气！"
                );
            break;
    }
}

void GameWindow::failToCombatMessage() {
    QMessageBox::information(this,"操作失败","请先派出战斗英雄");
}

void GameWindow::traitActivateShow(bool w,bool m,bool a) {
    QString wa = w?"True":"False";
    QString ma = m?"True":"False";
    QString as = a?"True":"False";

    QMessageBox::information(this,"羁绊激活",
        QString("------本次出战，激活羁绊：--------\n"
                "战士羁绊： %1\n"
                "法师羁绊： %2\n"
                "刺客羁绊： %3\n")
                .arg(wa).arg(ma).arg(as)
        );
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