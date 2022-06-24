#pragma once

#include "gamemem.h"


namespace Game {
    // MAJOR_RELEASE.MINOR_RELEASE.RELEASE.BUILD
    string version = "0.0.1.1";
    
    int createNewSave() {

        int result = -1;
        int saveNum = 0;
        bool saving = true;

        while (saving) {

            //create save folder
            string location = "mkdir \Saves\\" + std::to_string(saveNum);
            if (std::system(location.c_str()) == false) {

                //populate save folder
                //header
                string headerloc = "Saves/" + std::to_string(saveNum) + "/header.dat";
                std::ofstream header(headerloc);
                if (header.is_open()) {

                    header << "VERSION " << version << endl;

                    header.close();
                }

                std::system((location + "\\chunks").c_str());
                std::system((location + "\\entity").c_str());
                std::system((location + "\\entity\\player").c_str());


                result = saveNum;
                saving = false;
            }
            else {
                saveNum++;
            }
        }

        return result;
    }




}