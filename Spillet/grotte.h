#ifndef GROTTE_H
#define GROTTE_H

#include "fjende.h"
#include <vector>
#include <string>

class Grotte
{
private:
    int id;
    std::string navn;
    std::vector<int> fjendeIDer;
    int guldSkat;


public:
    Grotte(int id, const std::string& navn, const std::vector<int>& fjendeIDer, int guldSkat);

    int getID() const;
    const std::string getNavn() const;
    const std::vector<int>& getFjendeIDer() const;
    int getGuldSkat() const;

    static std::vector<Grotte> loadGrotter(sqlite3* db);
};

#endif // GROTTE_H
