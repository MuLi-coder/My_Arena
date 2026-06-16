//
// Created by ml on 2026/6/16.
//

#ifndef MY_ARENA_SWORD_H
#define MY_ARENA_SWORD_H
#include "../Equipment.h"


class Sword : public Equipment{
public:
    Sword(){
        name = "Sword";
        image = "../images/Sword.png";
        hpBuff = 0;
        attBuff = 15;
        attSpeedBuff = 1;
        attAreaBuff=0;
        moveSpeedBuff=0;
        manaBuff=0;
    }
};


#endif //MY_ARENA_SWORD_H