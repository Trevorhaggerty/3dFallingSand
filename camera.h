#pragma once


#include "world.h"

namespace Game {
	

	//camera object that will be the eyes and ears of the user
	struct camera {

		//constructor
		camera(loc3d loc, loc3df rot, loc3d dim) { location = loc; rotation = rot; dimensions = dim; logger.updateLocation("camera"); };

		//logging class
		Logger logger;

		//attributes
		loc3d location;
		loc3df rotation;
		
		//x,y are dimensions, z is draw distance
		loc3d dimensions;

		//main rendering function
		void renderVoxels(region* rgn, float* pixels);
	};

    





}



