//
// Created by ml on 2026/6/16.
//

#ifndef MY_ARENA_CHAINMAIL_H
#define MY_ARENA_CHAINMAIL_H
#include "../Equipment.h"


class ChainMail : public Equipment{
public:
    ChainMail(){
        name = "ChainMail";
        image = "../images/ChainMail.png";
        hpBuff = 30;
        attBuff = 0;
        attSpeedBuff = 0;
        attAreaBuff = 0;
        moveSpeedBuff = 0;
        manaBuff = 0;
    }
};


#endif //MY_ARENA_CHAINMAIL_H
