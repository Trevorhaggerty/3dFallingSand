#pragma once

#include "substance.h"


namespace Game {
    
    std::ostream& operator <<(std::ostream& out, const substance& s) {
        out << s.name;
        return out;
    }
	//load in the substance dictionary




}




