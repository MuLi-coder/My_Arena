#ifndef PIECEWIDGET_H
#define PIECEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "../entity/Unit.h" // 引入你的 Unit 基类或 Knight 等头文件

class PieceWidget : public QWidget {
    Q_OBJECT
public:
    explicit PieceWidget(QWidget *parent = nullptr);
    // 提供一个接口，让外界把 Unit 数据塞进来
    void setUnit(Unit* unit);

private:
    QLabel* upLabel;
    QLabel* hpLabel;   // 血条
    QLabel* hpEmptyLabel;
    QLabel* manaLabel;
    QLabel* manaEmptyLabel;
    QLabel* imageLabel;   // 人物图片
    QLabel* levelLabel;   // 等级
    Unit* currentUnit;    // 记录当前绑定的逻辑棋子
    const int CELL_SIZE = 60;
};

#endif // PIECEWIDGET_H