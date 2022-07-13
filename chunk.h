#pragma once

//includes
#include "includes.h"
#include "voxels.h"
#include "colorfuncs.h"

const int RDIM = 12;
namespace Game {
    


    /*Chunk class
        stores and manipulates voxel data
        the backbone of the games system
    */
    class chunk {

    public:
        chunk(int x =0, int y=0, int z=0);

    private:

        voxel* voxels[RDIM][RDIM][RDIM];
        loc3d location;

    public:
        voxel getVoxel(int x, int y, int z) { return *(voxels[x][y][z]); };
        int DrawVoxels(float verts[], loc3d curs, int offset);
        // void UpdateNeighbors();
        void Simulate();

        void Fill(voxel v);
        void Insert(voxel * v, int x, int y, int z);

        void SetLocation(loc3d loc) { location = loc; };

        void Save(std::string saveloc);
        void Load(std::string loadloc,int x, int y, int z);

        ~chunk();
    };



    int randomInt(int i);
}
