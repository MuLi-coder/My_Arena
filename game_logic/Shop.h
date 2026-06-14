//
// Created by ml on 2026/6/12.
//

#ifndef MY_ARENA_SHOP_H
#define MY_ARENA_SHOP_H

#include <vector>
#include "../entity/Unit.h"
class Shop {
    std::vector<Unit*> shop;
    std::vector<int> intShop;

public:
    Shop();
    void placeUnitAt(int s,Unit* unit);
    void removeUnitAt(int s);
    Unit* getUnitAt(int s) const;
    bool isCellEmpty(int s) const;

};

#endif //MY_ARENA_SHOP_H