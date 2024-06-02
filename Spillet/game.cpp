
#include "game.h"
#include <iostream>
#include <vector>
#include <algorithm>

Game::Game() : db(nullptr) {}

Game::~Game() {
    if (db) {
        sqlite3_close(db);
    }
}

int Game::initDatabase() {
    int rc = sqlite3_open("/home/sammy/Documents/Spillet/Spillet.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Kan ikke åbne databasen: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }
    return SQLITE_OK;
}

void Game::loadGrotter() {
    grotter = Grotte::loadGrotter(db);
}

void Game::loadFjender() {
    fjender = fjende::loadFjender(db);
}

Hero Game::createHero() {
    std::string navn;
    std::cout << "Skriv et Hero navn: ";
    std::cin >> navn;
    Hero hero(navn, 1, 0, 10, 2, 2, 0);
    return hero;
}

Hero Game::loadHero() {
    std::string navn;
    std::cout << "Skriv et Hero navn, for at loade: ";
    std::cin >> navn;
    return Hero::loadHero(db, navn);
}

void Game::mainMenu(Hero &hero) {
    while (true) {
        std::cout << "1. Kæmp imod fjende\n2. Gå ind i Grotte\n3. Gem og Luk\nVælg en mulighed: ";
        int valg;
        std::cin >> valg;

        if (valg == 1) {
            std::vector<fjende> fjender = fjende::loadFjender(db);
            if (fjender.empty()) {
                std::cerr << "Ingen fjender loaded fra databasen." << std::endl;
                return;
            }

            std::cout << "Vælg en fjende at kæmpe imod\n";
            for (size_t i = 0; i < fjender.size(); ++i) {
                std::cout << i + 1 << ". " << fjender[i].getNavn() << "HP: " << fjender[i].getHP() << ", Skade: " << fjender[i].getSkade() << ", XP: " << fjender[i].getXP() << ")\n";
            }

            int fjendeIndex;
            std::cin >> fjendeIndex;
            if (fjendeIndex > 0 && fjendeIndex <= fjender.size()) {
                battle(hero, fjender[fjendeIndex - 1]);
            } else {
                std::cerr << "Ugyldigt valg." << std::endl;
            }
        } else if(valg == 2) {
            std::cout << "Vælg en Grotte at gå ind i:\n";
            for(size_t i = 0; i < grotter.size(); ++i) {
                std::cout << (i + 1) << ". " << grotter[i].getNavn() << "\n";
            }
            int grotteChoice;
            std::cin >> grotteChoice;
            if (grotteChoice > 0 && grotteChoice <= grotter.size()) {
                enterGrotte(hero, grotter[grotteChoice - 1]);
            } else {
                std::cout << "Ugyldigt valg.\n";
            }

        } else if(valg == 3) {
            hero.saveHero(db);
            break;
        } else {
            std::cerr << "Ugyldigt valg." << std::endl;
        }
    }
}


void Game::battle(Hero& hero, const fjende& Fjende) {
    fjende currentfjende = Fjende;

    while (hero.getHP() > 0 && currentfjende.getHP() > 0) {
        std::cout << "Hero angriber " << currentfjende.getNavn() << " for " << hero.getSkade() << " skade!\n";
        currentfjende.setHP(currentfjende.getHP() - hero.getSkade());
        if (currentfjende.getHP() > 0) {
            std::cout << currentfjende.getNavn() << " angriber hero for " << currentfjende.getSkade() << " skade!\n";
            hero.setHP(hero.getHP() - currentfjende.getSkade());
        }
    }

    if (hero.getHP() > 0) {
        std::cout << "Hero besejrede " << currentfjende.getNavn() << " og fik " << currentfjende.getXP() << "XP!\n";
        hero.addXP(currentfjende.getXP());
    } else {
        std::cout << "Hero var besejret af " << currentfjende.getNavn() << "...\n";
    }
}

void Game::enterGrotte(Hero &hero, const Grotte &grotte) {
    std::cout << "Går ind i: " << grotte.getNavn() << "...\n";

    for(int fjendeID : grotte.getFjendeIDer()){
       fjende* foundFjende = nullptr;
       for (fjende& Fjende : fjender) {
           if (Fjende.getID() == fjendeID) {
               foundFjende = &Fjende;
               break;
           }
       }

       if (foundFjende) {
           battle(hero, *foundFjende);

           if(hero.getHP() <= 0) {
               std::cout << "Hero var besejret i " << grotte.getNavn() << ".\n";
               return;
           }
       }
    }
    std::cout << "Hero klarede " << grotte.getNavn() << " og fik " << grotte.getGuldSkat() << " guldmønter!\n";
    hero.addGuld(grotte.getGuldSkat());
}

void Game::start() {
    if (initDatabase() != SQLITE_OK) {
        std::cerr << "Fejlede at initialisere databasen." << std::endl;
        return;
    }

    loadFjender();
    loadGrotter();

    std::cout << "1. Skab en ny hero\n2. Load eksisterende Hero\nVælg en mulighed: ";
    int valg;
    std::cin >> valg;

    Hero hero("", 0, 0, 0, 0, 0, 0);

    if (valg == 1) {
        hero = createHero();
    } else if (valg == 2) {
        hero = loadHero();
    } else {
        std::cerr << "Ugyldigt valg." << std::endl;
        return;
    }

    mainMenu(hero);
}
