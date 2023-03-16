#include "../include/Studio.h"
#include <iostream>
#include <string>
#include <vector>
#include "../include/Action.h"

using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv){
   if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
        }
    extern Studio* backup;
    string configurationFile =argv[1];
    Studio studio =Studio(configurationFile);
    studio.start();

    if(backup!=nullptr) {
        delete backup;
        backup = nullptr;
    }
   return 0;
}

