#pragma once

//includes
#include "includes.h"
#include "chunk.h"


namespace Game {
    


    /*region class
        stores and manipulates chunk data
        core class
    */


    struct region {

        region(loc3d loc = { 0,0,0 }) {
            location = loc;
            logger.updateLocation("region");
        };

        chunk* chunks[RDIM][RDIM][RDIM];
        loc3d location;
        Logger logger;

        void Generate();
        void Render();
        void Simulate();

        voxel GetVoxel(loc3d);
        chunk GetChunk(loc3d);
        
        void Save(std::string saveloc);
        void Load(std::string loadloc, int x, int y, int z);

        ~region();
    };

}
