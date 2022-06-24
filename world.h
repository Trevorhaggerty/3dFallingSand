#ifndef WORLD_H
#define	WORLD_H

#include "includes.h"

namespace Game
{
	
	class world {

	public:
		world() { ; };
	private:
		std::string name;

	public:
		void SetName(std::string n) { name = n; };
		void SayName() { std::cout << name << std::endl; };

	};

}



#endif