#ifndef GAME_H
#define GAME_H

#include "hero.h"
#include "fjende.h"
#include "grotte.h"
#include <vector>
#include <sqlite3.h>


class Game {

private:
  sqlite3 *db;
  std::vector<fjende> fjender;
  std::vector<Grotte> grotter;

  int initDatabase();
  void loadFjender();
  void loadGrotter();
  Hero createHero();
  Hero loadHero();
  void mainMenu(Hero &hero);
  void battle(Hero& hero, const fjende& Fjende);
  void enterGrotte(Hero& hero, const Grotte& grotte);



public:
  Game();
  ~Game();
  void start();


};

#endif // GAME_H
