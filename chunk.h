#pragma once

//includes
#include "includes.h"
#include "voxels.h"



namespace Game {
    


    /*Chunk class
        stores and manipulates voxel data
        the backbone of the games system
    */
    struct chunk {

        chunk(loc3d loc = { 0,0,0 });


        voxel * voxels[RDIM][RDIM][RDIM];
        loc3d location;
        Logger logger;

        unsigned int vtoi(voxel v) {

            unsigned int vi = 0;

            vi = v.subId;
            vi += v.temperature * 1000;
            vi += v.settle_timer * 10000;
            vi += v.flags * 100000;
            vi += v.settled * 1000000000;

            return vi;
        };


        voxel itov(unsigned int vi) {

            voxel v;

            v.subId = vi % 1000;
            v.temperature = floor((vi / 1000) % 10);
            v.settle_timer = floor((vi / 10000) % 10);
            v.settled = floor(vi / 1000000000);
            v.flags = floor((vi / 100000) % 10000);

            return v;
        }

        voxel GetVoxel(loc3d loc);

        void Simulate();
        void Fill(voxel* v);
        void Insert(voxel * v, int x, int y, int z);

        void SetLocation(loc3d loc) { location = loc; };

        void Save(std::string saveloc);
        void Load(std::string loadloc,int x, int y, int z);

        ~chunk();
    };



    int randomInt(int i);
}
