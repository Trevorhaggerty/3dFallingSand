
#include "colorfuncs.h"



namespace Game {

	loc4df blend(loc4df a, loc4df b) {
		loc4df result;

		result.x = b.x * (b.w * 1.0f) + a.x * (1.0f - b.w);
		result.y = b.y * (b.w * 1.0f) + a.y * (1.0f - b.w);
		result.z = b.z * (b.w * 1.0f) + a.z * (1.0f - b.w);

		return result;
	};


}