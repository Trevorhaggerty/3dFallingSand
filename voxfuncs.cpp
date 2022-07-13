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

    loc3d findAtHeight(loc2d mouseCurs, int height) {
        loc3d result;

        result.x = height - mouseCurs.y + (mouseCurs.x / 2);
        result.y = height;
        result.z = height - mouseCurs.y - (mouseCurs.x / 2);


        return result;
    }


}


