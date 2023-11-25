#pragma once

#include "includes.h"


//custom logger for managing information and trounleshooting
namespace Game {
    
    //tne main logger class
    class Logger {
    
    public:  
        
        //constructor
        Logger(std::string l = "unknown", int f = DEBUGVALUE, int c = 0,   bool d = true, bool s = true) {
            location = l;
            filter = f;
            displaying = d;
            saving = s;

        };
        
   
    private:
        //internal variables
        std::string location;
        int filter;
        bool displaying;
        bool saving;
        static int counter;

       

    public:
        //main logging function
        int log(int priority, std::string event, std::string message);


        //mutators
        void updateLocation(std::string l) { location = l; };
        void updateDisplaying(bool b) { displaying = b; };
        



    };




}