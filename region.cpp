#pragma once

#include "region.h"


namespace Game {

	chunk region::GetChunk(loc3d loc){
		chunk result;

		if (loc.x >= 0 && loc.x < RDIM * RDIM && loc.y >= 0 && loc.y < RDIM * RDIM && loc.z >= 0 && loc.z < RDIM * RDIM) {
			return result;
		}
		else {
			int dim = RDIM;
			loc = loc / dim;
			return *chunks[loc.x][loc.y][loc.z];
		}

	}


	void region::Generate(){
		for (int i = 0; i < (RDIM); i++) {
			for (int j = 0; j < (RDIM); j++) {
				for (int k = 0; k < (RDIM); k++) {
					chunks[i][j][k]->SetLocation(loc3d(i, j, k));
				}
			}
		}	
	}


	voxel region::GetVoxel(loc3d loc) {
		int dim = RDIM;
		chunk temp = GetChunk(loc);
		return temp.GetVoxel(loc);


	}


}

