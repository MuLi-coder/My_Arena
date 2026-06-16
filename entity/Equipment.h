//
// Created by ml on 2026/6/16.
//

#ifndef MY_ARENA_EQUIPMENT_H
#define MY_ARENA_EQUIPMENT_H

#include<QString>


class Equipment {
protected:
    QString name;
    QString image;
    int hpBuff;
    int attBuff;
    int attSpeedBuff;
    int attAreaBuff;
    int moveSpeedBuff;
    int manaBuff;
public:
    Equipment();
    QString getName()const;
    QString getImage()const;
    int getHpBuff()const;
    int getAttBuff()const;
    int getAttSpeedBuff()const;
    int getAttAreaBuff()const;
    int getMoveSpeedBuff()const;
    int getManaBuff()const;

};


#endif //MY_ARENA_EQUIPMENT_H