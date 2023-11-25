#pragma once

#include "chunk.h"


int rdim = 12;
namespace Game {

    
    void chunk::Insert(voxel* v, int x, int y, int z) 
    { 
        if (x >= 0 && x < rdim && y >= 0 && y < rdim && z >= 0 && z < rdim) {
            voxels[x][y][z] = v;
        }
    };

    int randomInt(int i) { return std::rand() % i; }


   
    chunk::chunk(loc3d loc) {
        location = loc;
        for (int i = 0; i < rdim; i++) {
            for (int j = 0; j < rdim; j++) {
                for (int k = 0; k < rdim; k++) {
                    voxels[i][j][k] = nullptr;
                }
            }
        }

    };

    void chunk::Fill(voxel* v) {
        for (int x = 0; x < RDIM; x++) {
            for (int y = 0; y < RDIM; y++) {
                for (int z = 0; z < RDIM; z++) {
                    voxels[x][y][z] = v;
                }
            };
        }
    };

    voxel chunk::GetVoxel(loc3d loc) {

        if (loc.x  >= (RDIM * this->location.x) && loc.x < RDIM + (RDIM * this->location.x) && loc.y >= 0 + (RDIM * this->location.y) && loc.y < RDIM + (RDIM * this->location.y) && loc.z >= 0 + (RDIM * this->location.z) && loc.z < RDIM + (RDIM * this->location.z)){
            voxel blank;
            return blank;
            //return *(voxels[loc.x - (RDIM * location.x)][loc.y - (RDIM * location.y)][loc.z - (RDIM * location.z)]);
         }
        else {
            voxel blank;
            return blank;
        }
    };

    void chunk::Simulate() {
        bool checking = true;
        int choice = 0;

        bool moved = false;

        bool posxlim = false;
        bool negxlim = false;
        bool poszlim = false;
        bool negzlim = false;

        loc3d cursor(0, 0, 0);

        std::vector<loc3d> unsettled;
        std::vector<loc3d> available;

        unsettled.clear();
        unsettled.shrink_to_fit();

        for (int x = 0; x <= rdim-1; x++) {
            for (int y = 0; y <= rdim - 1; y++) {
                for (int z = 0; z <= rdim - 1; z++) {
                    if (voxels[x][y][z]->settled == false) {
                        unsettled.push_back(loc3d(x, y, z));
                    }
                }
            }
        }


        std::random_shuffle(unsettled.begin(), unsettled.end(), randomInt);

        for (std::vector<loc3d>::iterator step = unsettled.begin(); step != unsettled.end(); ++step) {
            //cout << *it;

            moved = false;

            if (voxels[step->x][step->y][step->z]->substance->state == "powder"
                || voxels[step->x][step->y][step->z]->substance->state == "liquid"
                || voxels[step->x][step->y][step->z]->substance->state == "gas") {
                if (step->y >= 0) {
                    if (step->y > 0) {
                        if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x][step->y - 1][step->z]->substance->density) {
                            voxels[step->x][step->y - 1][step->z]->settled = false;
                            std::swap(voxels[step->x][step->y][step->z], voxels[step->x][step->y - 1][step->z]);
                            moved = true;
                        }
                        else {
                            available.clear();
                            available.shrink_to_fit();

                            negxlim = (step->x <= 0);
                            posxlim = (step->x >= rdim - 1);
                            negzlim = (step->z <= 0);
                            poszlim = (step->z >= rdim - 1);

                            //loop through the bottom 9 (clipping based on edges)
                            if (!negxlim && !negzlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x - 1][step->y - 1][step->z - 1]->substance->density) {
                                    available.push_back(loc3d(step->x - 1, step->y - 1, step->z - 1));
                                }
                            }
                            if (!negxlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x - 1][step->y - 1][step->z]->substance->density) {
                                    available.push_back(loc3d(step->x - 1, step->y - 1, step->z));
                                }
                            }
                            if (!negxlim && !poszlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x - 1][step->y - 1][step->z + 1]->substance->density) {
                                    available.push_back(loc3d(step->x - 1, step->y - 1, step->z + 1));
                                }
                            }

                            if (!posxlim && !negzlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x + 1][step->y - 1][step->z - 1]->substance->density) {
                                    available.push_back(loc3d(step->x + 1, step->y - 1, step->z - 1));
                                }
                            }
                            if (!posxlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x + 1][step->y - 1][step->z]->substance->density) {
                                    available.push_back(loc3d(step->x + 1, step->y - 1, step->z));
                                }
                            }
                            if (!posxlim && !poszlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x + 1][step->y - 1][step->z + 1]->substance->density) {
                                    available.push_back(loc3d(step->x + 1, step->y - 1, step->z + 1));
                                }
                            }
                            if (!negzlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x][step->y - 1][step->z - 1]->substance->density) {
                                    available.push_back(loc3d(step->x, step->y - 1, step->z - 1));
                                }
                            }
                            if (!poszlim) {
                                if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x][step->y - 1][step->z + 1]->substance->density) {
                                    available.push_back(loc3d(step->x, step->y - 1, step->z + 1));
                                }
                            }

                            if (available.size() == 0 && voxels[step->x][step->y][step->z]->substance->state == "liquid") {
                                if (!negxlim && !negzlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x - 1][step->y][step->z - 1]->substance->density) {
                                        available.push_back(loc3d(step->x - 1, step->y, step->z - 1));
                                    }
                                }
                                if (!negxlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x - 1][step->y][step->z]->substance->density) {
                                        available.push_back(loc3d(step->x - 1, step->y, step->z));
                                    }
                                }
                                if (!negxlim && !poszlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x - 1][step->y][step->z + 1]->substance->density) {
                                        available.push_back(loc3d(step->x - 1, step->y, step->z + 1));
                                    }
                                }

                                if (!posxlim && !negzlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x + 1][step->y][step->z - 1]->substance->density) {
                                        available.push_back(loc3d(step->x + 1, step->y, step->z - 1));
                                    }
                                }
                                if (!posxlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x + 1][step->y][step->z]->substance->density) {
                                        available.push_back(loc3d(step->x + 1, step->y, step->z));
                                    }
                                }
                                if (!posxlim && !poszlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x + 1][step->y][step->z + 1]->substance->density) {
                                        available.push_back(loc3d(step->x + 1, step->y, step->z + 1));
                                    }
                                }
                                if (!negzlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x][step->y][step->z - 1]->substance->density) {
                                        available.push_back(loc3d(step->x, step->y, step->z - 1));
                                    }
                                }
                                if (!poszlim) {
                                    if (voxels[step->x][step->y][step->z]->substance->density > voxels[step->x][step->y][step->z + 1]->substance->density) {
                                        available.push_back(loc3d(step->x, step->y, step->z + 1));
                                    }
                                }
                            }
                            if (available.size() != 0) {
                                std::random_shuffle(available.begin(), available.end(), randomInt);
                                cursor = available.front();
                                voxels[step->x][step->y][step->z]->settled = false;
                                voxels[step->x][step->y][step->z]->settle_timer = 0;

                                voxels[cursor.x][cursor.y][cursor.z]->settled = false;
                                voxels[cursor.x][cursor.y][cursor.z]->settle_timer = 0;
                                std::swap(voxels[step->x][step->y][step->z], voxels[cursor.x][cursor.y][cursor.z]);
                                moved = true;
                                
                            }

                        }
                    }
                }
            }
            voxels[step->x][step->y][step->z]->checkSettle(moved);
        }

    };

    










    void chunk::Save(std::string saveloc) {
    
        saveloc = saveloc + std::to_string(location.x) + "," + std::to_string(location.y) + "," + std::to_string(location.z) + ".chunk";
            std::fstream chunkFile(saveloc, std::ios::out|std::ios::trunc|std::ios::binary);
            if (chunkFile.is_open()) {

                for (int x = 0; x < rdim; x++) {
                    for (int y = rdim - 1; y >= 0; y--) {
                        for (int z = 0; z < rdim; z++) {
                            chunkFile << *(voxels[x][y][z]) << std::endl;
                        }
                        chunkFile << std::endl;
                    }
                chunkFile << std::endl;
                }
                            
                 

                chunkFile.close();
            }

    };




    void chunk::Load(std::string loadloc, int x, int y, int z) {
    
        loadloc = loadloc + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ".chunk";
        std::fstream chunkFile(loadloc, std::ios::in | std::ios::binary);
        if (chunkFile.is_open()) {

            for (int x = 0; x < rdim; x++) {
                for (int y = 0; y < rdim; y++) {
                    for (int z = 0; z < rdim; z++) {
                        //chunkFile >> voxels[x][y][z];
                    }
                }
            }

            chunkFile.close();
        }

    
    };



    chunk::~chunk() {


    }









}

