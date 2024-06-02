cmake_minimum_required(VERSION 3.5)

project(Spillet LANGUAGES CXX)

find_package(SQLite3 REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Spillet main.cpp
    hero.h hero.cpp
    fjende.h fjende.cpp
    battle.h battle.cpp
    game.h game.cpp
    grotte.h grotte.cpp)

target_link_libraries(Spillet PRIVATE
SQLite::SQLite3)


include(GNUInstallDirs)
install(TARGETS Spillet
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
