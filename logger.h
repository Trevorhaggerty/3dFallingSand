#pragma once

#include "includes.h"


//custom logger
namespace logspace {

    class Logger {
    
    public:  
        Logger(std::string l = "unknown", int f = 10, int c = 0, bool d = true, bool s = true) {
            location = l;
            filter = f;
            counter = c;
            displaying = d;
            saving = s;
        };
    private:
        std::string location;
        int filter;
        bool displaying;
        bool saving;
        int counter;

    public:
        int log(int priority, std::string event, std::string message);
        void updateLocation(std::string l) { location = l; };
    };




}