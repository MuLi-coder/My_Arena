#include "PieceWidget.h"

PieceWidget::PieceWidget(QWidget *parent) : QWidget(parent), currentUnit(nullptr) {
    this->setStyleSheet("border: 0.1px solid gray;");//直接去除边框
    // 1. 创建垂直布局，把血条和图片上下拼起来
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 2, 5, 2); // 留出一点边距，显得不那么挤
    layout->setSpacing(3); // 血条和图片的间距

    // 2. 创建血条
    hpBarLabel = new QLabel(this);
    hpBarLabel->setFixedHeight(6);
    hpBarLabel->setStyleSheet("background-color: #289e09; border-radius:3px; border: green");
    hpBarLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true); //允许鼠标穿透

    // 3. 创建图片 Label
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter); // 图片居中
    imageLabel->setStyleSheet("background: transparent");
    imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true); //允许鼠标穿透
    // 4. 把血条和图片加入布局
    layout->addWidget(hpBarLabel);
    layout->addWidget(imageLabel);
    this->hide();
}


// 核心方法：绑定逻辑数据，并更新界面
void PieceWidget::setUnit(Unit* unit) {
    this->setStyleSheet("border: 0.1px solid gray;");//直接去除边框
    currentUnit = unit;
    if (unit) {
        // 1. 加载并缩放人物图片
        QPixmap pixmap(unit->getImage());
        QPixmap scaledPixmap = pixmap.scaled(CELL_SIZE, CELL_SIZE - 8, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 高度减去血条的大概高度
        imageLabel->setPixmap(scaledPixmap);

        // 2. 根据当前血量更新血条样式（比如满血绿，残血红）
        int hpPercent = unit->getHp()*100/ unit->getMaxHp();
        hpBarLabel->setFixedSize((CELL_SIZE-10) * hpPercent/100, 6);

        hpBarLabel->setVisible(true);
        imageLabel->setVisible(true);
        this->show();

    } else {
        this->setFixedSize(CELL_SIZE, CELL_SIZE+8);
        this->show();
        hpBarLabel->setVisible(false);
        imageLabel->setVisible(false);
    }
}