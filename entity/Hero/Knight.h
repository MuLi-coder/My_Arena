//
// Created by ml on 2026/5/26.
//

#ifndef MY_ARENA_KNIGHT_H
#define MY_ARENA_KNIGHT_H

#include<iostream>
#include<string>
#include "../Unit.h"
#include<QString>

class Knight : public Unit{
private:
    int armor;
    int att;
    QString image;
public:
    Knight(std::string n, int pos,int owner = 0);

    void attack(Unit* target) override;

    QString getImage() const override;
};


#endif //MY_ARENA_KNIGHT_H