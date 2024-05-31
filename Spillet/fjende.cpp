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
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
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




/*
void fjende::create(const std::string &navn, int HP, int Skade, int XP) {
    this->navn = navn;
    this->HP = HP;
    this->Skade = Skade;
    this->XP = XP;
}

void fjende::saveToDatabase(sqlite3* db) {
    std::string sql = "INSERT INTO fjende (navn, HP, Skade, XP) VALUES (?, ?, ?, ?,)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK) {
        std::cerr << "Faild to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, HP);
    sqlite3_bind_int(stmt, 3, Skade);
    sqlite3_bind_int(stmt, 4, XP);

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

fjende fjende::loadFromDatabase(sqlite3* db, const std::string& navn) {
    std::string sql = "SELECT HP, Skade, XP, FROM fjende WHERE name = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK) {
        std::cerr << "Faild to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return fjende();
    }
    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_ROW) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        return fjende();
    }

    int HP = sqlite3_column_int(stmt, 0);
    int Skade = sqlite3_column_int(stmt, 1);
    int XP = sqlite3_column_int(stmt, 2);

    sqlite3_finalize(stmt);

    return fjende(navn,HP,Skade, XP);
}
*/

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
/*
void fjende::printInfo() const {
    std::cout << "Navn: " << navn << std::endl;
    std::cout << "HP: " << HP << std::endl;
    std::cout << "Skade: " << Skade << std::endl;
}
*/
