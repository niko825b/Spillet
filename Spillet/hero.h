#ifndef HERO_H
#define HERO_H

#include <string>
#include <sqlite3.h>


class Hero {

private:
    int id;
    std::string navn;
    int XP;
    int Level;
    int HP;
    int Skade;
    int Styrke;
    int Guld;

    sqlite3* db;


public:
    Hero(std::string navn);

    Hero(const std::string &navn, int XP, int Level, int HP, int Skade, int Styrke, int Guld);

    Hero(int id, const std::string& navn, int XP, int Level, int HP, int Skade, int Styrke, int Guld);

    static Hero loadHero(sqlite3* db, const std::string& navn);
    bool saveHero(sqlite3* db);



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

    int getGuld() const;
    void setGuld(int Guld);

    void addXP(int XP);
    void levelUp();
    void addGuld(int Guld);

};

#endif // HERO_H
