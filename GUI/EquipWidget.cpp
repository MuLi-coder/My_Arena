//
// Created by ml on 2026/6/16.
//

#include "EquipWidget.h"

#include <QVBoxLayout>

EquipWidget::EquipWidget(QWidget* parent):QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(0);

    currentEquipment = nullptr;
    equipLabel = new QLabel(this);
    equipLabel->setAlignment(Qt::AlignCenter);
    equipLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    equipLabel->setStyleSheet("background: transparent");
    layout->addWidget(equipLabel);
    this->hide();
}

void EquipWidget::setEquip(Equipment* equip) {
    currentEquipment = equip;
    if (currentEquipment) {
        QPixmap pixmap(currentEquipment->getImage());
        QPixmap scaledPixmap = pixmap.scaled(CELL_SIZE-4, CELL_SIZE-4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        equipLabel->setPixmap(scaledPixmap);
        this->setStyleSheet("background: transparent");
        equipLabel->setVisible(true);
        this->show();
    }else {
        this->setFixedSize(CELL_SIZE,CELL_SIZE);
        this->show();
        equipLabel->setVisible(false);
    }

}
