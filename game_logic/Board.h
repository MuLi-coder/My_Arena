#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "../entity/Unit.h"
class Board {

private:
    //场上整个board实例，包括棋盘和备战区两个部分
    int rows_, cols_,ben_;
    std::vector<std::vector<Unit*>> grid_;
    std::vector<Unit*> bench_;

public:
    Board(int rows = 7, int cols = 7,int ben =6);

    ~Board();

    // 判断某个格子是否为空,空返回 true,越界或者已占用，返回false
    bool isCellEmpty(int row, int col) const;
    bool isCellEmpty(int pos) const;

    // 获取指定格子的元素
    Unit* getUnitAt(int row, int col) const;
    Unit* getUnitAt(int pos) const;

    // 在指定格子放置一个指定棋子
    void placeUnitAt(int row, int col, Unit* hero);
    void placeUnitAt(int pos,Unit* hero);

    // 悬空指定格子
    void removeUnitAt(int row, int col);
    void removeUnitAt(int pos);

    //获取棋盘和备战区大小
    int getRows() const;
    int getCols() const;
    int getBen() const;
};

#endif // BOARD_H