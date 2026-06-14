//
// Created by ml on 2026/6/13.
//

#include "ParaWidget.h"

#include <qboxlayout.h>

ParaWidget::ParaWidget(QWidget * parent): QWidget(parent) {
    this->setStyleSheet("background-color: white");
    //主控件
    //主布局，水平布局，三个部分
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    //----------------左边主控件---------------
    QWidget* leftPart = new QWidget(this);
    leftPart->setStyleSheet("background-color: white");
    // leftPart->setFixedSize(CELL_SIZE*2.5,2*CELL_SIZE);
    mainLayout->addWidget(leftPart);

    QVBoxLayout* leftLayout = new QVBoxLayout(leftPart);
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(0);
    leftLayout->setAlignment(Qt::AlignCenter);
    //左侧两个Label
    //1--玩家标签
    playerLabel = new QLabel(this);
    playerLabel->setStyleSheet("color:black ;background-color: white");
    playerLabel->setText("PLAYER_SCORE: 0");
    playerLabel->setAlignment(Qt::AlignCenter);
    playerLabel->setFixedSize(130,60);
    //2--等级
    levelLabel = new QLabel(this);
    levelLabel->setStyleSheet("color:black;background-color: white");
    levelLabel->setText(QString("LEVEL: 0"));
    levelLabel->setAlignment(Qt::AlignCenter);
    levelLabel->setFixedSize(130,60);
    //将两个标签加入左侧布局
    leftLayout->addWidget(playerLabel);
    leftLayout->addWidget(levelLabel);

    //---------------中间主控件-------------
    QWidget* middlePart = new QWidget(this);
    middlePart->setStyleSheet("background-color: white");
    // middlePart->setFixedSize(CELL_SIZE*2.5,2*CELL_SIZE);
    mainLayout->addWidget(middlePart);

    QVBoxLayout* middleLayout = new QVBoxLayout(middlePart);
    middleLayout->setContentsMargins(0,0,0,0);
    middleLayout->setSpacing(0);
    middleLayout->setAlignment(Qt::AlignCenter);
    //中间两个Label
    //1---血量
    hpLabel = new QLabel(middlePart);
    hpLabel->setStyleSheet("color:black;background-color: white");
    hpLabel->setText(QString("HP: 0"));
    hpLabel->setAlignment(Qt::AlignCenter);
    hpLabel->setFixedSize(130,60);
    //2---金币
    moneyLabel = new QLabel(middlePart);
    moneyLabel->setStyleSheet("color:black;background-color: white");
    moneyLabel->setText(QString("MONEY: 50"));
    moneyLabel->setAlignment(Qt::AlignCenter);
    moneyLabel->setFixedSize(130,60);
    //加入中间布局
    middleLayout->addWidget(hpLabel);
    middleLayout->addWidget(moneyLabel);

    //---------------右侧主控件-------------------
    QWidget* rightPart = new QWidget(this);
    rightPart->setStyleSheet("background-color: white");
    // rightPart->setFixedSize(CELL_SIZE*2.5,2*CELL_SIZE);
    mainLayout->addWidget(rightPart);

    QVBoxLayout* rightLayout = new QVBoxLayout(rightPart);
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSpacing(0);
    rightLayout->setAlignment(Qt::AlignCenter);
    //右侧两个Label
    //1----剩余刷新商店次数
    shopRefreshLabel = new QLabel(this);
    shopRefreshLabel->setStyleSheet("color:black;background-color: white");
    shopRefreshLabel->setText(QString("SHOP_REFRESH: 3"));
    shopRefreshLabel->setAlignment(Qt::AlignCenter);
    shopRefreshLabel->setFixedSize(130,60);
    //2----上阵人数
    maxUnitLabel = new QLabel(this);
    maxUnitLabel->setStyleSheet("color:black;background-color: white");
    maxUnitLabel->setText(QString("MAX_UNITS: 3"));
    maxUnitLabel->setAlignment(Qt::AlignCenter);
    maxUnitLabel->setFixedSize(130,60);
    //加入右侧布局
    rightLayout->addWidget(shopRefreshLabel);
    rightLayout->addWidget(maxUnitLabel);
}

void ParaWidget::setPara(Player *player) {
    playerLabel->setText(QString("PLAYER_SCORE: %1").arg(player->getScore()));
    levelLabel->setText(QString("LEVEL: %1").arg(player->getLevel()));
    hpLabel->setText(QString("HP: %1").arg(player->getHp()));
    moneyLabel->setText(QString("MONEY: %1").arg(player->getMoney()));
    shopRefreshLabel->setText(QString("SHOP_REFRESH: %1").arg(player->getShopRefreshTimes()));
    maxUnitLabel->setText(QString("MAX_UNITS: %1").arg(player->getMaxUnits()));
}
