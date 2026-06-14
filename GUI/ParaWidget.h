//
// Created by ml on 2026/6/13.
//

#ifndef MY_ARENA_PARAWIDGET_H
#define MY_ARENA_PARAWIDGET_H

#include <QWidget>
#include <QLabel>
#include "../game_logic/Player.h"

class ParaWidget : public QWidget {
    Q_OBJECT
    QLabel* playerLabel;
    QLabel* levelLabel;
    QLabel* hpLabel;
    QLabel* moneyLabel;
    QLabel* shopRefreshLabel;
    QLabel* maxUnitLabel;
    const int CELL_SIZE = 60;
public:
    explicit ParaWidget(QWidget* parent = nullptr);
    void setPara(Player* player);
};


#endif //MY_ARENA_PARAWIDGET_H