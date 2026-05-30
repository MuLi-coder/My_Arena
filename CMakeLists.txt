cmake_minimum_required(VERSION 3.16)
project(my_Arena LANGUAGES CXX)

# 1. 找到 Qt 库 (这里假设你用的是 Qt6，如果是 Qt5 把 6 改成 5)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# 2. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 开启 Qt 的自动 moc 功能（让 CMake 自动处理 Q_OBJECT 宏）
set(CMAKE_AUTOMOC ON)
# 2. 开启 Qt 的自动 rcc 功能（如果以后用到 qrc 资源文件，也需要这个）
set(CMAKE_AUTORCC ON)

# 3. 定义我们要生成的可执行文件
add_executable(my_Arena
        main.cpp
        entity/Unit.cpp
        entity/Unit.h
        entity/Hero/Knight.cpp
        entity/Hero/Knight.h
        entity/Hero/Mage.cpp
        entity/Hero/Mage.h
        game_logic/Board.cpp
        game_logic/Board.h
        GUI/GameWindow.cpp
        GUI/GameWindow.h
        GUI/PieceWidget.cpp
        GUI/PieceWidget.h
        game_logic/GameManager.cpp
        game_logic/GameManager.h
)

# 4. 把 Qt 的库链接到我们的程序上
target_link_libraries(my_Arena PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)
