#pragma once

//input map struct
struct imap {

    //all the keys of the keyboard
    bool key[104];
    //all the possible buttons on the mouse
    bool mousekeys[12];
    //the x and y of the mouses position
    double mousex;
    double mousey;

    int mousei;

    //constructor
    imap() {

        for (int n = 0; n < 104; n++) {
            key[n] = false;
        }
        for (int n = 0; n < 12; n++) {
            mousekeys[n] = false;
        }

        mousex = 0;
        mousey = 0;

        mousei = 0;
    };
};

