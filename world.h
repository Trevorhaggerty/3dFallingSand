#pragma once

#include "includes.h"
#include "region.h"

namespace Game
{
	
	class world {
	public:
		world() {
			name = "world";
			Id = "0";
			seed = 0;
			//todo get dates and times down
			creationDate = "current date";
			regioncount = 0;
		};
		world(std::string n, std::string id = "0", int s = 0) {
			name = n;
			Id = id;
			seed = s;
			//todo get dates and times down
			creationDate = "current date";
			regioncount = 0;
		};
	private:
		std::string name;
		std::string Id;
		int seed;
		std::vector<region> * rgns;
		int regioncount;
		std::vector<int>regionIndex;
		std::string creationDate;
	
	public:
		//mutators

		//memory management

		void createRegion() {};

		void SaveWorld();
		void LoadWorld();
		void DeleteWorld();

		void GetVoxelAt(loc3d);
		void SetVoxelAt(loc3d);

		void Simulate(loc3d);
	};

}


