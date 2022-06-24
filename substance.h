#pragma once

#include "includes.h"

namespace Game
{
    struct substance {

        substance(std::string n, substance* ls, substance* hs, float l, float h, float c[3], float d, std::string s) {
            name = n;
            hiSub = hs;
            loSub = ls;
            hiThresh = h;
            loThresh = l;
            std::copy(c, c + 3, color);
            density = d;
            state = s;
        };
        std::string name;

        //substances that it transforms to from temperature change
        substance* hiSub;
        substance* loSub;

        //the temperatures that cause change
        float hiThresh;
        float loThresh;

        //the color of the substance
        float color[4];

        //this determines priority motion for particles
        float density;

        //this determines the gradient that is available for movement
        std::string state;

        friend std::ostream& operator<<(std::ostream& out, const substance& s);
    };
    

    

    

}
