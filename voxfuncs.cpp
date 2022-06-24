#pragma once
#include "voxfuncs.h"
namespace Game{
    loc3d indexToCoord(int index, int dim) {
        loc3d result(0, 0, 0);

        result.x = index / (dim * dim);
        result.y = (index / dim) % dim;
        result.z = index % dim;

        return result;
    }

}


