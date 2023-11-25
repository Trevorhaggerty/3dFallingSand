#pragma once
#include "camera.h";

namespace Game {

	struct session {

		session() {};

		std::string ID;
		world* World;
		camera* Camera;
	};

}