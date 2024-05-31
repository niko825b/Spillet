#include "game.h"

int main() {

    Game game;
    game.start();

    /*
    int choice;
    bool quit = false;

    while(!quit) {
        //Vis hovedmenu
        std::cout << "Main Menu:" << std::endl;
        std::cout << "1. Load spil" << std::endl;
        std::cout << "2. Create ny hero" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Skriv dit valg: ";
        std::cin >> choice;

        switch (choice) {
            case 1: //load game
                game.load();
            break;

            case 2: //Create ny hero
                game.createHero();
            break;

            case 3: //Quit
                quit = true;
            break;

            default:
                std::cout << "Ugyldigt valg. Prøv igen" << std::endl;
            break;
        }

        if(!quit) {
            //Vis submenu for spilaktivitet
            while(true) {
                std::cout << "\nGame Menu:" << std::endl;
                std::cout << "1. Kæmp mod fjender" << std::endl;
                std::cout << "2. Gem hero" << std::endl;
                std::cout << "3. Quit game" << std::endl;
                std::cout << "Skriv dit valg: ";
                std::cin >> choice;

                switch (choice) {
                    case 1: //kæmp mod fjender
                        game.battle();
                    break;

                    case 2: //Gem hero
                        game.save();
                    break;

                    case 3: //Quit
                        quit = true;
                    break;

                    default:
                        std::cout << "Ugyldigt valg. Prøv igen" << std::endl;
                    break;
                }

                if(quit || choice == 3) {
                    break;
                }
            }
        }
    }
    */

    return 0;
}
