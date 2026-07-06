#include "PieceWidget.h"

PieceWidget::PieceWidget(QWidget *parent) : QWidget(parent), currentUnit(nullptr) {
    this->setStyleSheet("border: 0.1px solid gray;");//直接去除边框
    // 1. 创建垂直布局，把血条和图片上下拼起来
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 4, 2); // 留出一点边距，显得不那么挤
    layout->setSpacing(0); // 血条和图片的间距

    upLabel = new QLabel(this);
    upLabel->setFixedSize(CELL_SIZE-4, 14);
    upLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    QHBoxLayout *upLayout = new QHBoxLayout(upLabel);
    upLayout->setContentsMargins(0, 0, 0, 0);
    upLayout->setSpacing(2);
    // 2. 创建等级
    levelLabel = new QLabel(this);
    levelLabel->setFixedSize(12,15);
    levelLabel->setStyleSheet("color:black ; border: 1px solid gray;");
    levelLabel->setAlignment(Qt::AlignCenter);
    levelLabel->setText(QString("1"));
    levelLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    // 2. 创建血条:上方血条，下方蓝条
    QLabel* uprightPart = new QLabel(upLabel);
    uprightPart->setFixedSize(CELL_SIZE-8, 8);
    QVBoxLayout* uprightLayout = new QVBoxLayout(uprightPart);
    uprightLayout->setContentsMargins(0, 0, 0, 0);
    uprightLayout->setSpacing(0);
    //右上第一部分，血条
    QLabel* hpBarLabel = new QLabel(uprightPart);
    hpBarLabel->setFixedSize(CELL_SIZE-8,6);
    QHBoxLayout *hpBarLayout = new QHBoxLayout(hpBarLabel);
    hpBarLayout->setContentsMargins(0, 0, 0, 0);
    hpBarLayout->setSpacing(0);
    hpBarLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //左半边血条
    hpLabel = new QLabel(hpBarLabel);
    hpLabel->setFixedWidth(CELL_SIZE-8);
    hpLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //右半边空条
    hpEmptyLabel = new QLabel(hpBarLabel);
    hpEmptyLabel->setFixedWidth(0);
    hpEmptyLabel->setStyleSheet("background-color: transparent");
    hpEmptyLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //加入血条布局
    hpBarLayout->addWidget(hpLabel);
    hpBarLayout->addWidget(hpEmptyLabel);

    //右上第二部分，蓝条
    QLabel* manaBarLabel = new QLabel(uprightPart);
    manaBarLabel->setFixedSize(CELL_SIZE-8,2);
    QHBoxLayout* manaBarLayout = new QHBoxLayout(manaBarLabel);
    manaBarLayout->setContentsMargins(0, 0, 0, 0);
    manaBarLayout->setSpacing(0);
    manaBarLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //左半边蓝条
    manaLabel = new QLabel(manaBarLabel);
    manaLabel->setFixedWidth(0);
    manaLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //右半边空条
    manaEmptyLabel = new QLabel(manaBarLabel);
    manaEmptyLabel->setFixedWidth(CELL_SIZE-8);
    manaEmptyLabel->setStyleSheet("background-color: transparent");
    manaEmptyLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    //加入蓝条布局
    manaBarLayout->addWidget(manaLabel);
    manaBarLayout->addWidget(manaEmptyLabel);

    //加入右上布局
    uprightLayout->addWidget(hpBarLabel);
    uprightLayout->addWidget(manaBarLabel);
    //加入上层布局
    upLayout->addWidget(levelLabel);
    upLayout->addWidget(uprightPart);

    // 3. 创建图片 Label
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter); // 图片居中
    imageLabel->setStyleSheet("background: transparent");
    imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents, true); //允许鼠标穿透
    // 4. 把血条和图片加入布局
    layout->addWidget(upLabel);
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
        QPixmap scaledPixmap = pixmap.scaled(CELL_SIZE, CELL_SIZE - 8, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);

        // 2. 根据当前血量更新血条
        int hpPercent = unit->getHp()*100/ unit->getMaxHp();
        hpLabel->setFixedSize((CELL_SIZE-8) * hpPercent/100, 6);
        hpEmptyLabel->setFixedSize(CELL_SIZE-8-(CELL_SIZE-8) * hpPercent/100, 6);
        if (currentUnit->getOwner()==0) {
            hpLabel->setStyleSheet("background-color: green; border-radius:3px; border: green");
        }else {
            hpLabel->setStyleSheet("background-color: red; border-radius:3px; border: red");
        }
        // 3.根据当前蓝量更新蓝条
        int manaPercent = unit->getMana()*100/ unit->getMaxMana();
        manaLabel->setFixedSize((CELL_SIZE-8) * manaPercent/100, 2);
        manaEmptyLabel->setFixedSize(CELL_SIZE-8-(CELL_SIZE-8) * manaPercent/100, 2);
        manaLabel->setStyleSheet("background-color: blue; border-radius:3px; border: blue");

        // 4. 如果有装备，那么背景变红
        if (currentUnit->isWearingEquipment()) {
            levelLabel->setStyleSheet("background-color: red;color:white");
        }else {
            levelLabel->setStyleSheet("border:1px solid gray;color:black");
        }

        //更新等级
        levelLabel->setText(QString("%1").arg(unit->getLevel()));
        levelLabel->setVisible(true);
        hpLabel->setVisible(true);
        manaLabel->setVisible(true);
        imageLabel->setVisible(true);
        this->show();

    } else {
        this->setFixedSize(CELL_SIZE, CELL_SIZE+8);
        this->show();
        levelLabel->setVisible(false);
        hpLabel->setVisible(false);
        manaLabel->setVisible(false);
        imageLabel->setVisible(false);
    }
}