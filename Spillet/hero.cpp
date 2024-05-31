#include "hero.h"
#include <iostream>



Hero::Hero(const std::string &navn, int XP, int Level, int HP, int Skade, int Styrke)
    : navn(navn), XP(XP), Level(Level), HP(HP), Skade(Skade), Styrke(Styrke) {}

Hero::Hero(const std::string &navn, int XP, int Level, int HP, int Skade, int Styrke, int id)
    : navn(navn), XP(XP), Level(Level), HP(HP), Skade(Skade), Styrke(Styrke), id(id){}

Hero Hero::loadHero(sqlite3* db, const std::string& navn) {
    Hero hero(" ", 0, 0, 0, 0, 0, 0);
    sqlite3_stmt* stmt;
    const char* sql = "SELECT navn, level, XP, HP, Skade, Styrke, id From Hero WHERE navn = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " <<  sqlite3_errmsg(db) << std::endl;
        return hero;
    }

    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_ROW) {
        hero = Hero(
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)),
                sqlite3_column_int(stmt, 1),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                sqlite3_column_int(stmt, 5),
                sqlite3_column_int(stmt, 6)
                );
    }

    sqlite3_finalize(stmt);
    return hero;
}

void Hero::saveHero(sqlite3* db) const {

    sqlite3_stmt* stmt;

    if(id) {
        const char* sql = "UPDATE Hero SET Level = ?, XP = ?, HP = ?, Skade = ?, Styrke = ? WHERE id = ?";

        if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare state: " << sqlite3_errmsg(db) << std::endl;
            return;
        }


        sqlite3_bind_int(stmt, 1, Level);
        sqlite3_bind_int(stmt, 2, XP);
        sqlite3_bind_int(stmt, 3, HP);
        sqlite3_bind_int(stmt, 4, Skade);
        sqlite3_bind_int(stmt, 5, Styrke);
        sqlite3_bind_int(stmt, 6, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {


    const char* sql = "INSERT INTO Hero (navn, Level, XP, HP, Skade, Styrke) VALUES (?, ?, ?, ?, ?, ?) ";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, Level);
    sqlite3_bind_int(stmt, 3, XP);
    sqlite3_bind_int(stmt, 4, HP);
    sqlite3_bind_int(stmt, 5, Skade);
    sqlite3_bind_int(stmt, 6, Styrke);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    }
}


/*
void Hero::saveToDatabase(sqlite3* db) {
    //Forbered SQL-forespørgsel
    std::string sql = "INSERT INTO Hero (navn, XP, Level, HP, Skade, Styrke) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK) {
        std::cerr << "Faild to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    //Bind parametre til SQL-forespørgslen
    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, XP);
    sqlite3_bind_int(stmt, 3, Level);
    sqlite3_bind_int(stmt, 4, HP);
    sqlite3_bind_int(stmt, 5, Skade);
    sqlite3_bind_int(stmt, 6, Styrke);

    //Udfør SQL-forespørgslen
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE) {
        std::cerr << "Faild to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    //Frigiv resourcer
    sqlite3_finalize(stmt);
}

Hero Hero::loadFromDatabase(sqlite3* db, const std::string& navn) {
    std::string sql = "SELECT XP, Level, HP, Skade, Styrke FROM Hero WHERE navn = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if(rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return Hero();
    }
    sqlite3_bind_text(stmt, 1, navn.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_ROW) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return Hero();
    }

    int XP = sqlite3_column_int(stmt, 0);
    int Level = sqlite3_column_int(stmt, 1);
    int HP = sqlite3_column_int(stmt, 3);
    int Skade = sqlite3_column_int(stmt, 4);
    int Styrke = sqlite3_column_int(stmt, 5);

    sqlite3_finalize(stmt);

    return Hero(navn, XP, Level, HP, Skade, Styrke);

}

*/

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

/*
void Hero::printInfo() const {
    std::cout << "Navn: " << navn << std::endl;
    std::cout << "XP: " << XP << std::endl;
    std::cout << "Level: " << Level << std::endl;
    std::cout << "HP: " << HP << std::endl;
    std::cout << "Styrke: " << Styrke << std::endl;
}

bool Hero::isAlive() const {
    return HP > 0;
}

int Hero::attack() const {
    //Implementer angrebslogik
    return Styrke; //Retuner et simpelt angrebsbeløb for nu
}

void Hero::takeDamage(int damage) {
    HP -= damage;
    if(HP < 0) HP = 0;
}
*/


void Hero::addXP(int XP){
    this->XP += XP;
    if (this->XP >= Level * 1000) {
        this->XP -= Level * 1000;
        levelUp();
    }
}

void Hero::levelUp() {
    Level++;
    HP += 2;
    Skade += 1;
    Styrke += 1;
}

/*
void Hero::levelUp() {
    int xpNeeded = Level *1000;
    if(XP >= xpNeeded) {
        Level++;
        XP -= xpNeeded;
        HP += 2;
        Skade++;
        Styrke++;
        std::cout << navn << " levled up to level " << Level << "!" << std::endl;
    } else {
        std::cout << " does not have enough XP to level up yet." << std::endl;
    }
}
*/








