#include "GUI/gamewindow.h" // 引入 GUI 文件夹下的 gamewindow.h
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 创建并显示游戏主窗口
    GameWindow w;
    w.show();

    return a.exec();
}