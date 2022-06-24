#pragma once

//includes
#include "includes.h"
#include "voxels.h"
#include "voxfuncs.h"


namespace Game {
    


    /*Chunk class
        stores and manipulates voxel data
        the backbone of the games system
    */
    class chunk {

    public:
        chunk();

    private:

        voxel voxels[12][12][12];


    public:
        voxel getVoxel(int x, int y, int z) { return voxels[x][y][z]; };
        void DrawVoxels(float verts[]);
        // void UpdateNeighbors();
        void Simulate();
        void Insert(voxel v, int x, int y, int z) { voxels[x][y][z] = v; };

        void Save(std::string saveloc,int x, int y , int z);
        void Load(std::string loadloc,int x, int y, int z);

        ~chunk();
    };




}
