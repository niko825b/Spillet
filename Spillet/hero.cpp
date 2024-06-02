#include "hero.h"
#include <iostream>

Hero::Hero(std::string navn)
    : id(0), navn(navn), HP(10), Level(1), XP(0), Skade(2), Styrke(2), Guld(0) {}

Hero::Hero(const std::string &navn, int XP, int Level, int HP, int Skade, int Styrke, int Guld)
    : navn(navn), XP(XP), Level(Level), HP(HP), Skade(Skade), Styrke(Styrke), Guld(Guld) {}

Hero::Hero(int id, const std::string &navn, int XP, int Level, int HP, int Skade, int Styrke, int Guld)
    : id(id), navn(navn), XP(XP), Level(Level), HP(HP), Skade(Skade), Styrke(Styrke), Guld(Guld) {}









Hero Hero::loadHero(sqlite3* db, const std::string& navn) {
    const char* sql = "SELECT navn, level, XP, HP, Skade, Styrke, id From Hero WHERE navn = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Klargøring fejlede: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);


    Hero hero(navn);
    if(sqlite3_step(stmt) == SQLITE_ROW) {
        hero = Hero(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                sqlite3_column_int(stmt, 5),
                sqlite3_column_int(stmt, 6),
                sqlite3_column_int(stmt, 7)
            );
    } else {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Hero blev ikke fundet");
    }

    sqlite3_finalize(stmt);
    return hero;
}

bool Hero::saveHero(sqlite3* db) {
    const char* sql;
    if (id == 0) {
       sql = "INSERT INTO Hero (navn, Level, XP, HP, Skade, Styrke, Guld) VALUES (?, ?, ?, ?, ?, ?, ?)";
    } else {
        "UPDATE Hero SET navn = ?, Level = ?, XP = ?, HP = ?, Skade = ?, Styrke = 0, Guld = ? WHERE id = ?";
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Klargøring fejlede: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }


    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, Level);
    sqlite3_bind_int(stmt, 3, XP);
    sqlite3_bind_int(stmt, 4, HP);
    sqlite3_bind_int(stmt, 5, Skade);
    sqlite3_bind_int(stmt, 6, Styrke);
    sqlite3_bind_int(stmt, 7, Guld);

    if (id != 0) {
        sqlite3_bind_int(stmt, 8, id);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Udførelse fejlede: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    if (id == 0) {
        id = static_cast<int>(sqlite3_last_insert_rowid(db));
    }

    sqlite3_finalize(stmt);
    return true;
}



std::string Hero::getNavn() const {
    return navn;
}
void Hero::setNavn(const std::string &navn) {
    this->navn = navn;
}

int Hero::getXP() const {
    return XP;
}
void Hero::setXP(int XP) {
    this->XP = XP;
}

int Hero::getLevel() const {
    return Level;
}
void Hero::setLevel(int Level) {
    this->Level = Level;
}

int Hero::getHP() const {
    return HP;
}
void Hero::setHP(int HP) {
    this->HP = HP;
}

int Hero::getStyrke() const {
    return Styrke;
}
void Hero::setStyrke(int Styrke) {
    this->Styrke = Styrke;
}

int Hero::getSkade() const {
    return Skade;
}
void Hero::setSkade(int Skade){
    this->Skade = Skade;
}



void Hero::addXP(int XP){
    this->XP += XP;
    if (this->XP >= Level * 1000) {
        levelUp();
    }
}

void Hero::levelUp() {
    XP = 0;
    Level++;
    HP += 2;
    Skade += 1;
    Styrke += 1;
}

void Hero::addGuld(int Guld) {
    this->Guld += Guld;
}



