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
    fjende(const std::string& navn, int HP, int Skade, int XP);

    static std::vector<fjende> loadFjender(sqlite3* db);

    //Getter og setter
    std::string getNavn() const;
    void setNavn(const std::string &navn);

    int getHP() const;
    void setHP(int HP);

    int getSkade() const;
    void setSkade(int Skade);

    int getXP() const;
    void setXP(int XP);

    int getID() const;
};

#endif // FJENDE_H
