#pragma once
//substance contains the struct for chemical substances and chemistry
#include "includes.h"
#include "logger.h"
#include "voxfuncs.h"

namespace Game
{
    //substances themselves
    struct substance {

        //constructors
        substance() {
            id = 0;
            name = "";
            hiSub = NULL;
            loSub = NULL;
            hiThresh = -INF;
            loThresh = INF;
            color = { 0.0f,0.0f,0.0f,0.f};
            density = 0;
            state = "";
            logger.updateLocation("substance");
            logger.log(1, "blank substance", "");
        };
        substance(int i, std::string n, substance* ls, substance* hs, float l, float h, loc4df c, float d, std::string s) {
            id = i;
            name = n;
            hiSub = hs;
            loSub = ls;
            hiThresh = h;
            loThresh = l;
            color = c;
            density = d;
            state = s;
            logger.updateLocation("substance");
            logger.log(1, "substance made", "");
        };

        int id;
        std::string name;

        //substances that it transforms to from temperature change
        substance* hiSub;
        substance* loSub;

        //the temperatures that cause change
        float hiThresh;
        float loThresh;

        //the color of the substance
        loc4df color;

        //this determines priority motion for particles
        float density;

        //this determines the gradient that is available for movement
        std::string state;

        //logging for substance creation
        Logger logger;

        friend std::ostream& operator<<(std::ostream& out, const substance& s);
    };
    

    

    

}
