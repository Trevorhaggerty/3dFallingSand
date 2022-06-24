#pragma once
#include "voxels.h"
namespace Game {

    std::ostream& operator <<(std::ostream& out, const voxel& v) {
        out << v.settled;
        //out << v.settle_timer;
        out << (*(v.substance));
        //out << v.temperature;
        return out;
    }



}