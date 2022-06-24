#ifndef WORLD_H
#define	WORLD_H

#include "includes.h"

namespace Game
{
	
	class world {

	public:
		world() { ; };
	private:
		string name;

	public:
		void SetName(string n) { name = n; };
		void SayName() { cout << name << endl; };

	};

}



#endif