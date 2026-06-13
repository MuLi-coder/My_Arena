//
// Created by ml on 2026/6/12.
//

#include "Shop.h"

Shop::Shop() {
    shop.resize(5,nullptr);
    intShop.resize(5,-1);
}

Unit *Shop::getUnitAt(const int s)const {
    if (s>=0 && s<5) {
        return shop[s];
    }
    return nullptr;
}

bool Shop::isCellEmpty(const int s) const{
    if (s>=0&&s<5) {
        return shop[s]==nullptr;
    }
    return false;
}

void Shop::placeUnitAt(const int s,Unit *unit) {
    shop[s]=unit;
}

void Shop::removeUnitAt(const int s) {
    shop[s]=nullptr;
}

std::vector<int> Shop::refresh() {
    for (int s=0; s<5; s++) {
        if (randomNum()<50) {
            intShop[s] = 0;
        }else {
            intShop[s] = 1;
        }
    }
    return intShop;
}