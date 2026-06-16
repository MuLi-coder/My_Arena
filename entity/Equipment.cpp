//
// Created by ml on 2026/6/16.
//

#include "Equipment.h"

Equipment::Equipment() {
    hpBuff=0;
    attBuff=0;
    attSpeedBuff=0;
    attAreaBuff=0;
    moveSpeedBuff=0;
    manaBuff=0;
}

QString Equipment::getName()const {
    return name;
}

QString Equipment::getImage()const {
    return image;
}

int Equipment::getHpBuff() const {
    return hpBuff;
}
int Equipment::getAttBuff() const {
    return attBuff;
}
int Equipment::getAttSpeedBuff() const {
    return attSpeedBuff;
}
int Equipment::getAttAreaBuff() const {
    return attAreaBuff;
}
int Equipment::getMoveSpeedBuff() const {
    return moveSpeedBuff;
}
int Equipment::getManaBuff() const {
    return manaBuff;
}

