#ifndef FJENDE_H
#define FJENDE_H

#include <string>
#include <sqlite3.h>
#include <vector>

class fjende {

private:

    sqlite3 *db;
    int id;
    std::string navn;
    int HP;
    int Skade;
    int XP;

public:
    /*
    fjende();
    */
    fjende(const std::string& navn, int HP, int Skade, int XP);

    static std::vector<fjende> loadFjender(sqlite3* db);

    /*
    void create(const std::string &navn, int HP, int Skade, int XP);
    void saveToDatabase(sqlite3* db);
    static fjende loadFromDatabase(sqlite3* db, const std::string& navn);
    */

    //Getter og setter
    std::string getNavn() const;
    void setNavn(const std::string &navn);

    int getHP() const;
    void setHP(int HP);

    int getSkade() const;
    void setSkade(int Skade);

    int getXP() const;
    void setXP(int XP);

    /*
    void printInfo() const;
    bool isAlive() const;
    int attack() const;
    void takeDamage(int Skade);
    */
};

#endif // FJENDE_H
