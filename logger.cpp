#pragma once
#include "logger.h"
#include <iomanip>
namespace Game {
	
   
    int Logger::counter = 0;

    int Logger::log(int priority,  std::string event, std::string message) {
        if (priority <= filter) {
            uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            if (counter == 0) {
                std::clog << std::setw(11) << "Event #" << " | ";
                std::clog << std::setw(11) << "Priority" << " | ";
                std::clog << std::setw(10) << "Time" << " | ";
                std::clog << std::setw(10) << "Scoping" << " | ";
                std::clog << std::setw(20) << "Event" << " | ";
                std::clog << std::setw(40) << "Message" << std::endl;
            }
            std::clog << std::setw(10) << "#:" << counter << " | ";
            std::clog << std::setw(10) << "Priority: " << priority << " | ";
            std::clog << std::setw(10) << currentTime << " | ";
            std::clog << std::setw(10) << location << " | ";
            std::clog << std::setw(20) << event << " | ";
            std::clog << std::setw(40) << message << std::endl;
        }

        counter++;
        
        return counter;
    };
       

}