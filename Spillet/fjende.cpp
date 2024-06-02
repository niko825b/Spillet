#include "fjende.h"
#include <iostream>
#include <sqlite3.h>


fjende::fjende(const std::string& navn, int HP, int Skade, int XP)
        : navn(navn), HP(HP), Skade(Skade), XP(XP) {}


std::vector<fjende> fjende::loadFjender(sqlite3* db) {
    std::vector<fjende> fjender;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT navn, HP, Styrke, XP FROM Fjende";

    if (sqlite3_prepare_v2(db,sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Klargøring fejlede: " << sqlite3_errmsg(db) << std::endl;
        return fjender;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string navn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int HP = sqlite3_column_int(stmt, 1);
        int Skade = sqlite3_column_int(stmt, 2);
        int XP = sqlite3_column_int(stmt, 3);
        fjender.emplace_back(navn, HP, Skade, XP);
    }

    sqlite3_finalize(stmt);
    return fjender;
}



//Setters og Getters

std::string fjende::getNavn() const {
    return navn;
}

void fjende::setNavn(const std::string &navn) {
    this->navn = navn;
}

int fjende::getHP() const {
    return HP;
}

void fjende::setHP(int HP) {
    this->HP = HP;
}

int fjende::getSkade() const {
    return Skade;
}

void fjende::setSkade(int Skade) {
    this->Skade = Skade;
}

int fjende::getXP() const {
    return XP;
}

void fjende::setXP(int XP) {
    this->XP =XP;
}

int fjende::getID() const{
    return id;
}
