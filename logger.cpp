#pragma once
#include "logger.h"
#include <iomanip>
namespace logspace {
	
   


    int Logger::log(int priority,  std::string event, std::string message) {
    
        time_t current_time;
        time(&current_time);
        if (counter == 0) {
            std::clog << std::setw(11) << "Event #" << " | ";
            std::clog << std::setw(11) << "Priority"<< " | ";
            std::clog << std::setw(10) << "Time" << " | ";
            std::clog << std::setw(10) << "Scoping" << " | ";
            std::clog << std::setw(10) << "Event" << " | ";
            std::clog << std::setw(10) << "Message" << std::endl;
        }
        std::clog << std::setw(10) << "#:" << counter << " | ";
        std::clog << std::setw(10) << "Priority: " << priority << " | ";
        std::clog << std::setw(10) << current_time << " | ";
        std::clog << std::setw(10) << location << " | " ;
        std::clog << std::setw(10) << event << " | ";
        std::clog << std::setw(10) << message << std::endl;
    
        counter++;
        return counter;
    };
       

}