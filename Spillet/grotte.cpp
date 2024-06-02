#include "grotte.h"
#include <sstream>
#include <iostream>

Grotte::Grotte(int id, const std::string& navn, const std::vector<int>& fjendeIDer, int guldSkat)
    : id(id), navn(navn), fjendeIDer(fjendeIDer), guldSkat(guldSkat) {}

int Grotte::getID() const{
    return id;
}

const std::string Grotte::getNavn() const {
    return navn;
}

const std::vector<int>& Grotte::getFjendeIDer() const {
    return fjendeIDer;
}

int Grotte::getGuldSkat() const{
    return guldSkat;
}

std::vector<Grotte> Grotte::loadGrotter(sqlite3 *db) {
    std::vector<Grotte> grotter;
    const char* sql = "SELECT id, navn, fjende_ider, guld_skat FROM grotte";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Fejl kunne ikke finde grotter: " << sqlite3_errmsg(db) << std::endl;
        return grotter;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string navn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string fjendeIDerStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int guldSkat = sqlite3_column_int(stmt, 3);

        std::vector<int> fjendeIDer;
        std::istringstream ss(fjendeIDerStr);
        std::string token;
        while (std::getline(ss, token, ',')){
            fjendeIDer.push_back(std::stoi(token));
        }

        grotter.push_back(Grotte(id, navn, fjendeIDer, guldSkat));
    }

    sqlite3_finalize(stmt);
    return grotter;
}
