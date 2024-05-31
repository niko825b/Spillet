#ifndef HERO_H
#define HERO_H

#include <string>
#include <sqlite3.h>


class Hero {

private:
    std::string navn;
    int XP;
    int Level;
    int HP;
    int Skade;
    int Styrke;
    int id = 0;
    sqlite3* db;


public:
    Hero(const std::string &navn, int XP, int Level, int HP, int Skade, int Styrke);

    Hero(const std::string& navn, int XP, int Level, int HP, int Skade, int Styrke, int id);

    static Hero loadHero(sqlite3* db, const std::string& navn);
    void saveHero(sqlite3* db) const;


    /*
    void saveToDatabase(sqlite3* db);
    static Hero loadFromDatabase(sqlite3* db, const std::string& navn);
    void levelUp();
    */

    //Setters og Getters
    std::string getNavn() const;
    void setNavn(const std::string &navn);

    int getXP() const;
    void setXP(int XP);

    int getLevel() const;
    void setLevel(int Level);

    int getHP() const;
    void setHP(int HP);

    int getStyrke() const;
    void setStyrke(int Styrke);

    int getSkade() const;
    void setSkade(int Skade);

    void addXP(int XP);
    void levelUp();


    /*
    void printInfo() const;
    bool isAlive() const;
    int attack() const;
    void takeDamage(int damage);
    bool checkLevelUp();
    void addXP();
    */
};

#endif // HERO_H
