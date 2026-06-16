//
// Created by ml on 2026/6/16.
//

#ifndef MY_ARENA_RAPIDGLOVES_H
#define MY_ARENA_RAPIDGLOVES_H
#include "../Equipment.h"


class RapidGloves : public Equipment{
public:
    RapidGloves(){
        name = "RapidGloves";
        image = "../images/RapidGloves.png";
        hpBuff = 0;
        attBuff = 0;
        attSpeedBuff = 2;
        attAreaBuff = 0;
        moveSpeedBuff = 1;
        manaBuff = 0;
    }
};


#endif //MY_ARENA_RAPIDGLOVES_H
