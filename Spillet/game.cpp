#include "game.h"
#include <iostream>
#include <vector>

Game::Game() : db(nullptr) {}

Game::~Game() {
    if (db) {
        sqlite3_close(db);
    }
}

int Game::initDatabase() {
    int rc = sqlite3_open("/home/sammy/Documents/Spillet/Spillet.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }
/*
    const char *sql_Hero = "CREATE TABLE IF NOT EXISTS Hero ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "navn TEXT NOT NULL, "
                           "Level INTEGER, "
                           "XP INTEGER, "
                           "HP INTEGER, "
                           "Skade INTEGER, "
                           "Styrke INTEGER)";

    const char *sql_Fjende = "CREATE TABEL IF NOT EXISTS Fjende ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "navn TEXT NOT NULL, "
                             "HP INTEGER, "
                             "Skade INTEGER, "
                             "XP INTEGER);";

    char *errMsg = nullptr;

    rc = sqlite3_exec(db, sql_Hero, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (Hero table): " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return rc;
    }

    rc = sqlite3_exec(db, sql_Fjende, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (Fjende table): " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return rc;
    }
*/
    return SQLITE_OK;
}

void Game::loadFjender() {
    fjender = fjende::loadFjender(db);
}

Hero Game::createHero() {
    std::string navn;
    std::cout << "Enter hero navn: ";
    std::cin >> navn;
    Hero hero(navn, 1, 0, 10, 2, 2);
    return hero;
}

Hero Game::loadHero() {
    std::string navn;
    std::cout << "Enter hero navn to load: ";
    std::cin >> navn;
    return Hero::loadHero(db, navn);
}

void Game::mainMenu(Hero &hero) {
    while (true) {
        std::cout << "1. Fight a fjende\n2. Save and Exit\nChoose an option: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::vector<fjende> fjender = fjende::loadFjender(db);
            if (fjender.empty()) {
                std::cerr << "No fjender loaded from database." << std::endl;
                return;
            }

            std::cout << "Choose an enemy to fight\n";
            for (size_t i = 0; i < fjender.size(); ++i) {
                std::cout << i + 1 << ". " << fjender[i].getNavn() << "HP: " << fjender[i].getHP() << ", Skade: " << fjender[i].getSkade() << ", XP: " << fjender[i].getXP() << ")\n";
            }

            int fjendeIndex;
            std::cin >> fjendeIndex;
            if (fjendeIndex > 0 && fjendeIndex <= fjender.size()) {
                battle(hero, fjender[fjendeIndex - 1]);
            } else {
                std::cerr << "Invalid choice." << std::endl;
            }
        } else if(choice == 2) {
            hero.saveHero(db);
            break;
        } else {
            std::cerr << "Invalid choice." << std::endl;
        }
    }
}


void Game::battle(Hero& hero, const fjende& Fjende) {
    fjende currentfjende = Fjende;

    while (hero.getHP() > 0 && currentfjende.getHP() > 0) {
        std::cout << "Hero attacks " << currentfjende.getNavn() << " for " << hero.getSkade() << " skade!\n";
        currentfjende.setHP(currentfjende.getHP() - hero.getSkade());
        if (currentfjende.getHP() > 0) {
            std::cout << currentfjende.getNavn() << " attacks hero for " << currentfjende.getSkade() << " skade!\n";
            hero.setHP(hero.getHP() - currentfjende.getSkade());
        }
    }

    if (hero.getHP() > 0) {
        std::cout << "Hero defeated " << currentfjende.getNavn() << " and gained " << currentfjende.getXP() << "XP!\n";
        hero.addXP(currentfjende.getXP());
    } else {
        std::cout << "Hero was defated by " << currentfjende.getNavn() << "...\n";
    }
}


void Game::start() {
    if (initDatabase() != SQLITE_OK) {
        std::cerr << "Failed to initialize database." << std::endl;
        return;
    }

    std::cout << "1. Create new hero\n2. Load existing hero\nChoose an option: ";
    int choice;
    std::cin >> choice;

    Hero hero("", 0, 0, 0, 0, 0);

    if (choice == 1) {
        hero = createHero();
    } else if (choice == 2) {
        hero = loadHero();
    } else {
        std::cerr << "Invalid choice." << std::endl;
        return;
    }

    mainMenu(hero);
}


/*
Game::Game(){
        initDatabase();

        if (sqlite3_prepare_v2(db, "SELECT navn, HP, Styrke, XP FROM Fjende", -1, &stmt, &errmsg)){
          std::cerr << sqlite3_errmsg(db);
        } else {
            while (sqlite3_step(stmt) != SQLITE_DONE) {
                fjender.push_back(fjende((std::string)reinterpret_cast<const char*>( sqlite3_column_text(stmt, 0)), sqlite3_column_int(stmt, 1), sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3)));
            }
            sqlite3_finalize(stmt);
        }


        hero =  nullptr;
            }


Game::~Game() {
    if(hero) {
        delete hero;
    }
    sqlite3_close(db);
}

void Game::start() {
    int choice;
    do {
        std::cout << "1. Create New Hero \n2. Load existing Hero\n3. Exit\nChoice: " << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 1:
                createHero();
            break;

            case 2:
                loadGame();
            break;

            case 3:
                std::cout << "Exiting...\n" << std::endl;
            break;

            default:
                std::cout << "Invalid choice, try again.\n" << std::endl;
        }
        break;
    } while (choice != 3);

    if (hero) {
        bool valid = 0;
        do {
            std::cout << "1. Chosse Enemy to battle\n2. Save Hero\n3. Exit\nCHoice: " << std::endl;
            std::cin >> choice;
            switch (choice) {
                case 1:
                    valid = 1;
                    chooseFjende();
                break;

                case 2:
                    valid = 1;
                    saveGame();
                break;

                case 3:
                    valid = 1;
                    return;
                break;

                default:
                    std::cout << "Invalid choice, try again. \n" << std::endl;
                break;
            }
        } while (!valid);
    }
}

void Game::loadGame() {
    loadHeroes();
    displayHeroes();
    int choice;
    std::cout << "Choose a hero to load: " << std::endl;
    std::cin >> choice;

    const char *sql = "SELECT navn, Level, XP, HP, Skade, Styrke FROM hero WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing select: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, choice);

    if (sqlite3_step(stmt) == SQLITE_ROW){
        std::string navn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        int Level = sqlite3_column_int(stmt, 1);
        int XP = sqlite3_column_int(stmt, 2);
        int HP = sqlite3_column_int(stmt, 3);
        int Skade = sqlite3_column_int(stmt, 4);
        int Styrke = sqlite3_column_int(stmt, 5);

        hero = new Hero(navn, Level, XP, HP, Skade, Styrke);
        std::cout << "Hero loaded: " << hero->getNavn() << std::endl;
    } else {
        std::cerr << "No hero found with id: " << choice << std::endl;
    }

    sqlite3_finalize(stmt);
}

void Game::saveGame(){
    if (!hero){
        std::cerr << "Ne hero to save. \n";
        return;
    }

    const char *sql = "INSERT INTO hero (navn, Level, XP, HP, Skade, Styrke) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing insert: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, hero->getNavn().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, hero->getLevel());
    sqlite3_bind_int(stmt, 3, hero->getXP());
    sqlite3_bind_int(stmt, 4, hero->getHP());
    sqlite3_bind_int(stmt, 5, hero->getSkade());
    sqlite3_bind_int(stmt, 6, hero->getStyrke());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting hero: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Hero saved successfully. \n";
    }

    sqlite3_finalize(stmt);
}

void Game::battle(fjende &fjende) {
    while (hero->getHP() > 0 && fjende.getHP() > 0) {
        fjende.setHP(fjende.getHP() - hero->getSkade());
        if(fjende.getHP() <= 0) {
            std::cout << "Enemy " << fjende.getNavn() << " defeated!\n";
            hero->gainXP(fjende.getXP());
            return;
        }
        hero->setHP(hero->getHP() - fjende.getSkade());
        if (hero->getHP() <= 0) {
            std::cout << "Hero " << hero->getNavn() << " defeated!\n";
            return;
        }
    }
}

void Game::createHero() {
    std::string navn;
    std::cout << "Enter hero navn: ";
    std::cin >> navn;
    hero = new Hero(navn);
}

void Game::chooseFjende() {
    int index = 1;
    for (const auto &fjende : fjender) {
        std::cout << index++ << ". " << fjende.getNavn() << " (HP: " <<fjende.getHP() << ", Skade: " << fjende.getSkade() << ")\n";
    }
    std::cout << "Choose fjende to battle: ";
    std::cin >> index;
    if(index > 0 && index <= fjender.size()) {
        battle(fjender[index - 1]);
    } else {
        std::cout << "Invalid choice\n";
    }
}
int Game::initDatabase() {
    int rc = sqlite3_open("../../Spillet", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;

        return rc;
    }
    return SQLITE_OK;
}
/*
void Game::loadFjender() {
    const char *sql = "INSERT INTO fjende (navn, HP, Skade, XP) VALUES "
                      "('Hest', 4, 1, 100), "
                      "('Weak Goblin', 4, 2, 200),"
                      "('Strong Goblin', 8, 3, 400),"
                      "('Stronger Goblin', 10, 4, 500),"
                      "('Den stærkeste Goblin', 15, 5, 800),"
                      "('Abe Kongen', 30, 5, 1000),"
                      "('Enhjærning', 5, 8, 1500),"
                      "('Drage', 100, 10, 3000),";
    char *errmsg;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errmsg) != SQLITE_OK) {
        std::cerr << "Error inserting fjender: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }

    sql = "SELECT navn, HP, Skade, XP FROM fjende;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing select: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        std::string navn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        int HP = sqlite3_column_int(stmt, 1);
        int Skade = sqlite3_column_int(stmt, 2);
        int XP = sqlite3_column_int(stmt, 3);
        fjender.emplace_back(navn, HP, Skade, XP);
    }
    sqlite3_finalize(stmt);
}

*/
/*
void Game::loadHeroes() {
    const char *sql = "SELECT id, navn, Level, XP, HP, Skade, Styrke FROM hero;";
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing select: "<< sqlite3_errmsg(db) << std::endl;
        return;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string navn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        int Level = sqlite3_column_int(stmt, 2);
        int XP = sqlite3_column_int(stmt, 3);
        int HP = sqlite3_column_int(stmt, 4);
        int Skade = sqlite3_column_int(stmt, 5);
        int Styrke = sqlite3_column_int(stmt, 6);
        std::cout << id << ". " << navn << " (Level: " << Level << ", XP: " << XP << ", HP: " << HP << ", Skade: " << Skade << ", Styrke: " << Styrke << ")\n";
    }
    sqlite3_finalize(stmt);
}

void Game::displayHeroes() {
    const char *sql = "SELECT id, navn FROM hero;";
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing select: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string navn = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::cout << id << ". " << navn << "\n";
    }
    sqlite3_finalize(stmt);
}
*/

/*
void Game::start() {

    int result = sqlite3_open("../../Spillet.db", &db);
    if(result != SQLITE_OK) {
        std::cerr << "Fejl ved åbning af databasen: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Databaseforbindelsen er oprettet" << std::endl;

        //Opret tabeller
        createHeroTable();
        createFjendeTable();

        //Udfyld fjendetabellen
        checkAndFillFjendeTabel();

        //SPillet er startet
    }
}


void Game::createHeroTable() {
    //Create Heroes table
    const char *createHeroTableSQL = "CREATE TABLE IF NOT EXISTS Hero ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "navn TEXT NOT NULL,"
                                      "XP INTEGER NOT NULL,"
                                      "Level INTEGER NOT NULL,"
                                      "HP INTEGER NOT NULL,"
                                      "Skade INTEGER NOT NULL,"
                                      "Styrke INTEGER NOT NULL"
                                      ")";

    char *errMsg = nullptr;
    int result = sqlite3_exec(db, createHeroTableSQL, nullptr, nullptr, &errMsg);
    if(result != SQLITE_OK) {
    std::cerr << "Fejl ved at oprette Hero tabel: " << errMsg << std::endl;
    sqlite3_free(errMsg);
    } else {
    std::cout << "Heroes table oprettet" << std::endl;
    }
}


void Game::createFjendeTable() {
    //Create Enemies table
    const char* createFjendeTableSQL = "CREATE TABLE IF NOT EXISTS fjende ("
                                             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                             "navn TEXT NOT NULL,"
                                             "HP INTEGER NOT NULL,"
                                             "Skade INTEGER NOT NULL,"
                                             "XP INTEGER NOT NULL,"
                                             ")";

    char* errMsg = nullptr;
    int result = sqlite3_exec(db, createFjendeTableSQL, nullptr, nullptr, &errMsg);
    if(result != SQLITE_OK) {
        std::cerr << "Fejl ved oprettelse af fjende tabel: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Fjende tabel oprettet" << std::endl;
    }
}

void Game::save() {
    //Gem helten i databasen
   hero.saveToDatabase(sqlite3* db);
}

void Game::load() {
    //indlæs en eksisterende helt fra databasen
    std::string heroNavn;
    std::cout << "SKriv heroens navn: ";
    std::cin >> heroNavn;
    Hero = Hero::loadFromDatabase(db, heroNavn);
}

void Game::createHero() {
    //Opret en ny hero
    std::string navn;
    std::cout << "Skriv heroens navn: ";
    std::cin >> navn;
    Hero Hero(navn, 0, 1, 10, 2, 2);
}

void Game::battle() {
    //Vælg en fjende at kæmpe imod
    showFjendeList();
    int choice;
    std::cout << "Vælg en fjende at kæmpe imod: ";
    std::cin >> choice;

    //Kontroller, om valget er gyldigt
    if(choice < 1 || choice > fjender.size()) {
        std::cout << "Ugyldigt valg" << std::endl;
        return;
    }

    //Start kampen
    fjende fjende = fjender[choice - 1];
    std::cout << "Du kæmper imod " << fjende.getNavn() << "!" << std::endl;

    //Simuler kampen
    while(Hero.getHP() > 0 && fjende.getHP() > 0) {
        //Heroen angriber fjenden
        int heroSkade = Hero.getStyrke();
        fjende.takeDamage(heroSkade);
        std::cout << "Du ramte " << fjende.getNavn() << " for " << heroSkade << "skade!" << std::endl;

        //Tjek om fjedne blev besejret
        if(fjende.getHP() <= 0) {
            std::cout << "Du besejrede " << fjende.getNavn() << "!" << std::endl;
            int xpEarend = fjende.getXP();
            hero.addXP(xpEarend);
            std::cout << "Du optjente " << xpEarend << " XP!" << std::endl;

            //CHeck om heroen stiger i level
            if(hero.checkLevelUp()) {
                std::cout << "Tillykke! Du er steget i level " << hero.getLevel() << "!" << std::endl;
            }

            break;
        }

        //Fjenden angriber
        int fjendeSkade = fjende.getDamage();
        hero.takeDamage(fjendeSkade);
        std::cout << fjende.getNavn() << " rammer dig for " << fjendeSkade << " skade!" << std::endl;

        //Tjek om heroen bliver besejret
        if(hero.getHP() <= 0) {
            std::cout << "Du er blevet besejret af " << fjende.getNavn() << "!" <<std::endl;
            break;
        }
    }
}

void Game::showFjendeList() {
    std::cout << "Fjende liste:" << std::endl;
    for(size_t i = 0; i < fjender.size(); ++i) {
        std::cout << i + 1 << ". " << fjender[i].getNavn() << std::endl;
    }
}
*/

/*
void Game::checkAndFillFjendeTabel() {
    //Tjek om fjende tabellen er tom
    int rowCount = 0;
    sqlite3_stmt* stmt;
    std::string query = "SELECT COUNT(*) FROM fjender;";

    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        std::cerr << "Fejl af klargøring: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    result = sqlite3_step(stmt);
    if(result == SQLITE_ROW) {
        rowCount = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if(rowCount == 0) {
        //Indsæt standard fjender i tabellen
        std::vector<std::string> defaultFjender = {
            "Hest",
            "Weak Goblin",
            "Stong Goblin",
            //TIlføj flere standard fjender senere
        };

        for(const std::string& fjendeNavn : defaultFjender) {
            query = "INSERT INTO fjender (navn, HP, Skade, XP) VALUES (?, ?, ?, ?);";
            result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if(result != SQLITE_OK) {
                std::cerr << "Fejl klargøring: " << sqlite3_errmsg(db) << std::endl;
                continue;
            }

            //Her skal der tilføjes logik til at bestemme HP, skade, og XP for hver fjende
            if(fjendeNavn == "hest") {
                HP = 4;
                Skade = 1;
                XP = 100;
            } else if(fjendeNavn == "Svag Goblin") {
                HP = 4;
                Skade = 2;
                XP = 200;
            } else if(fjendeNavn == "Stærk Goblin") {
                 HP = 8;
                Skade = 3;
                XP = 400;
            } else if(fjendeNavn == "Stærkerer Goblin"){
                HP = 10;
                Skade = 4;
                XP = 500;
            } else if(fjendeNavn == "Den stærkeste Goblin") {
                HP = 15;
                Skade = 5;
                XP = 800;
            } else if(fjendeNavn == "Abe Kongen") {
                HP = 30;
                Skade = 5;
                XP = 1000;
            } else if(fjendeNavn == "Enhjørning") {
                HP = 5;
                Skade = 8;
                XP = 1500;
            } else if(fjendeNavn == "Drage") {
                HP = 100;
                Skade = 8;
                XP = 3000;
            } else {
                //Håndter ukendt fjende
                std::cerr << "Ukendt fjende: " << fjendeNavn << std::endl;
            }


            sqlite3_bind_text(stmt, 1, fjendeNavn.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 2, /* HP *);
            sqlite3_bind_int(stmt, 3, /* skade *);
            sqlite3_bind_int(stmt, 4, /* XP *);

            result = sqlite3_step(stmt);
            if(result != SQLITE_DONE) {
                std::cerr << "Fejl ved indsætning af fjender: " << sqlite3_errmsg(db) << std::endl;
            }

            sqlite3_finalize(stmt);
        }

        std::cout << "Standard fjender er tilføjet til databasen" << std::endl;
    }
}
*/
























/*

void Game::showLoadingScreen() {
    std::cout << "Loading..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Loading complete!" << std::endl;

    //Vis valgmulighederne
    std::cout << "1. Create new hero" << std::endl;
    std::cout << "2. Load existing hero" << std::endl;
    std::cout << "3. Quit game" << std::endl;

    //Læs brugerens valg
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;

    //Afhængig af brugerens valg, udfør handlingen
    switch (choice) {
        case 1:
            createNewHero();
        break;

        case 2:
                loadExistingHero();
        break;

        case 3:
            std::cout << "Exiting game..." << std::endl;
        break;

        default:
            std::cout << "Invalid choice. Exiting game..." << std::endl;
        break;
    }
}


void Game::loadExistingHero() {
    //Implementer logik for at indlæse en eksisterende hero
    std::cout << "Loading existing hero..." << std::endl;

    //Gå til valg af fjende
    chooseFjende();
}

void Game::createNewHero() {
    //Implenenter logik for oprettelse af en ny hero
    std::cout << "Creating new hero..." << std::endl;

    //Gå til valg af fjende
    chooseFjende();
}

void Game::chooseFjendeScreen() {
    //Forbind til databasen
    sqlite3 *db;
    int rc = sqlite3_open("fjender.db", &db);
    if(rc) {
        std::cerr << "Cant open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::cout << "Choose an enemy to battle:" << std::endl;
    std::cout << "1. Goblin" << std::endl;
    std::cout << "2. Dragon" << std::endl;

    //Læs brugerens valg
    int choice;
    std::cout << "Enter choice: ";
    std::cin >> choice;

    //Opret en fjende baeret på brugerens valg
    fjende fjende;

    //Hent fjedens data fra databasen baseret på brugerens valg
    switch (choice) {
        case 1:
            fjende = fjende::loadFromDatabase(db, "Goblin");
        break;

        case 2:
            fjende = fjende::loadFromDatabase(db, "Dragon");
        break;

        default:
            std::cout << "Invaild choice. Exiting game..." << std::endl;
            sqlite3:close(db);
            return;
    }

    if(fjende.getNavn().empty()) {
        std::cout << "Enemy not found in the database." << std::endl;
        sqlite3_close(db);
        return;
    }

    std::cout << "You have chosen to battle " << fjende.getNavn() << "!" << std::endl;

    //luk forbindelsen til databasen
    sqlite3_close(db);
}

*/


