cmake_minimum_required(VERSION 3.16)
project(my_Arena LANGUAGES CXX)

# 2. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 找到 Qt 库 (这里假设你用的是 Qt6，如果是 Qt5 把 6 改成 5)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(my_Arena
        main.cpp
        entity/Unit.cpp
        entity/Unit.h
        entity/Hero/Knight.cpp
        entity/Hero/Knight.h
        entity/Hero/Mage.cpp
        entity/Hero/Mage.h
        GUI/GameWindow.cpp
        GUI/GameWindow.h
        GUI/PieceWidget.cpp
        GUI/PieceWidget.h
        game_logic/GameManager.cpp
        game_logic/GameManager.h
        game_logic/Player.cpp
        game_logic/Player.h
        game_logic/PreBoard.cpp
        game_logic/PreBoard.h
        game_logic/ComBoard.cpp
        game_logic/ComBoard.h
        game_logic/Shop.cpp
        game_logic/Shop.h
        GUI/ParaWidget.cpp
        GUI/ParaWidget.h
        GUI/EquipWidget.cpp
        GUI/EquipWidget.h
        entity/Equipment.cpp
        entity/Equipment.h
        entity/Equipment/Sword.h
        entity/Equipment/RapidGloves.h
        entity/Equipment/ChainMail.h
        entity/Equipment/BlueCrystal.h
        entity/Hero/Guardian.h
        entity/Hero/Guardian.cpp
        entity/Hero/Assassin.h
        entity/Hero/Assassin.cpp
        entity/Hero/Archer.h
        entity/Hero/Archer.cpp
        entity/Hero/Warlock.h
        entity/Hero/Warlock.cpp

)

# 4. 把 Qt 的库链接到我们的程序上
target_link_libraries(my_Arena PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

if(MSVC)
    # 强制开启 C++17 标准支持并修正 __cplusplus 宏 (解决 Qt 报错)
    # 使用 target_compile_options 确保只对 my_Arena 生效，且覆盖全局设置
    target_compile_options(my_Arena PRIVATE /Zc:__cplusplus /utf-8)

    # 同时确保 C++ 标准被正确设置（虽然 set(CMAKE_CXX_STANDARD 17) 通常够用，但显式指定更保险）
    target_compile_features(my_Arena PUBLIC cxx_std_17)

    message(STATUS "已应用 MSVC 修复补丁：开启 /Zc:__cplusplus 和 /utf-8")
endif()