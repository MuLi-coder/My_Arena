//
// Created by ml on 2026/6/16.
//

#ifndef MY_ARENA_BLUECRYSTAL_H
#define MY_ARENA_BLUECRYSTAL_H
#include "../Equipment.h"


class BlueCrystal : public Equipment{
public:
    BlueCrystal(){
        name = "BlueCrystal";
        image = "../images/BlueCrystal.png";
        hpBuff = 0;
        attBuff = 5;
        attSpeedBuff = 0;
        attAreaBuff = 0;
        moveSpeedBuff = 0;
        manaBuff = 2;
    }
};


#endif //MY_ARENA_BLUECRYSTAL_H
