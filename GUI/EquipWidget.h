//
// Created by ml on 2026/6/16.
//

#ifndef MY_ARENA_EQUIPWIDGET_H
#define MY_ARENA_EQUIPWIDGET_H
#include <QLabel>
#include "../entity/Equipment.h"

class EquipWidget : public QWidget {
    Q_OBJECT
    QLabel* equipLabel;
    Equipment* currentEquipment;
    const int CELL_SIZE = 60;
public:
    explicit EquipWidget(QWidget *parent = nullptr);
    void setEquip(Equipment* equip);
};


#endif //MY_ARENA_EQUIPWIDGET_H