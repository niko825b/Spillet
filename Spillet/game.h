#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include "fjende.h"
#include <sqlite3.h>
#include "hero.h"

class Game {
public:
    Game();
    ~Game();

    void start();
    int initDatabase();


    /*
    void showFjendeList();
    void checkAndFillFjendeTabel();
    void createHeroTable();
    void createFjendeTable();
    */

private:
  sqlite3 *db;
  std::vector<fjende> fjender;

  Hero createHero();
  Hero loadHero();
  void mainMenu(Hero &hero);
  void battle(Hero& hero, const fjende& Fjende);
  void loadFjender();

/*
    void showLoadingScreen();
    void loadExistingHero();
    void createNewHero();
    void chooseFjendeScreen();
    fjende chooseFjende();
*/
};

#endif // GAME_H
