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


   
    chunk::chunk(int x, int y , int z ) {
        location = { x,y,z };
        float basecolor[] = { 26 / 25500.0f, 107 / 25500.0f, 102 / 25500.0f , 0.01f};
        substance* air = new substance("air", NULL, NULL, -INF, INF, basecolor, 0.0001f, "gas");
        voxel* airVox = new voxel(air, 75.0f, false);
        for (int i = 0; i < rdim; i++) {
            for (int j = 0; j < rdim; j++) {
                for (int k = 0; k < rdim; k++) {
                    voxels[i][j][k] = airVox;
                }
            }
        }

    };

    void chunk::Fill(voxel v) {

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

        for (std::vector<loc3d>::iterator it = unsettled.begin(); it != unsettled.end(); ++it) {
            //cout << *it;

            moved = false;

            if (voxels[it->x][it->y][it->z]->substance->state == "powder"
                || voxels[it->x][it->y][it->z]->substance->state == "liquid"
                || voxels[it->x][it->y][it->z]->substance->state == "gas") {
                if (it->y >= 0) {
                    if (it->y > 0) {
                        if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x][it->y - 1][it->z]->substance->density) {
                            voxels[it->x][it->y - 1][it->z]->settled = false;
                            std::swap(voxels[it->x][it->y][it->z], voxels[it->x][it->y - 1][it->z]);
                            moved = true;
                        }
                        else {
                            available.clear();
                            available.shrink_to_fit();

                            negxlim = (it->x <= 0);
                            posxlim = (it->x >= rdim - 1);
                            negzlim = (it->z <= 0);
                            poszlim = (it->z >= rdim - 1);

                            //loop through the bottom 9 (clipping based on edges)
                            if (!negxlim && !negzlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x - 1][it->y - 1][it->z - 1]->substance->density) {
                                    available.push_back(loc3d(it->x - 1, it->y - 1, it->z - 1));
                                }
                            }
                            if (!negxlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x - 1][it->y - 1][it->z]->substance->density) {
                                    available.push_back(loc3d(it->x - 1, it->y - 1, it->z));
                                }
                            }
                            if (!negxlim && !poszlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x - 1][it->y - 1][it->z + 1]->substance->density) {
                                    available.push_back(loc3d(it->x - 1, it->y - 1, it->z + 1));
                                }
                            }

                            if (!posxlim && !negzlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x + 1][it->y - 1][it->z - 1]->substance->density) {
                                    available.push_back(loc3d(it->x + 1, it->y - 1, it->z - 1));
                                }
                            }
                            if (!posxlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x + 1][it->y - 1][it->z]->substance->density) {
                                    available.push_back(loc3d(it->x + 1, it->y - 1, it->z));
                                }
                            }
                            if (!posxlim && !poszlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x + 1][it->y - 1][it->z + 1]->substance->density) {
                                    available.push_back(loc3d(it->x + 1, it->y - 1, it->z + 1));
                                }
                            }
                            if (!negzlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x][it->y - 1][it->z - 1]->substance->density) {
                                    available.push_back(loc3d(it->x, it->y - 1, it->z - 1));
                                }
                            }
                            if (!poszlim) {
                                if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x][it->y - 1][it->z + 1]->substance->density) {
                                    available.push_back(loc3d(it->x, it->y - 1, it->z + 1));
                                }
                            }

                            if (available.size() == 0 && voxels[it->x][it->y][it->z]->substance->state == "liquid") {
                                if (!negxlim && !negzlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x - 1][it->y][it->z - 1]->substance->density) {
                                        available.push_back(loc3d(it->x - 1, it->y, it->z - 1));
                                    }
                                }
                                if (!negxlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x - 1][it->y][it->z]->substance->density) {
                                        available.push_back(loc3d(it->x - 1, it->y, it->z));
                                    }
                                }
                                if (!negxlim && !poszlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x - 1][it->y][it->z + 1]->substance->density) {
                                        available.push_back(loc3d(it->x - 1, it->y, it->z + 1));
                                    }
                                }

                                if (!posxlim && !negzlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x + 1][it->y][it->z - 1]->substance->density) {
                                        available.push_back(loc3d(it->x + 1, it->y, it->z - 1));
                                    }
                                }
                                if (!posxlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x + 1][it->y][it->z]->substance->density) {
                                        available.push_back(loc3d(it->x + 1, it->y, it->z));
                                    }
                                }
                                if (!posxlim && !poszlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x + 1][it->y][it->z + 1]->substance->density) {
                                        available.push_back(loc3d(it->x + 1, it->y, it->z + 1));
                                    }
                                }
                                if (!negzlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x][it->y][it->z - 1]->substance->density) {
                                        available.push_back(loc3d(it->x, it->y, it->z - 1));
                                    }
                                }
                                if (!poszlim) {
                                    if (voxels[it->x][it->y][it->z]->substance->density > voxels[it->x][it->y][it->z + 1]->substance->density) {
                                        available.push_back(loc3d(it->x, it->y, it->z + 1));
                                    }
                                }
                            }
                            if (available.size() != 0) {
                                std::random_shuffle(available.begin(), available.end(), randomInt);
                                cursor = available.front();
                                voxels[it->x][it->y][it->z]->settled = false;
                                voxels[it->x][it->y][it->z]->settle_timer = 0;

                                voxels[cursor.x][cursor.y][cursor.z]->settled = false;
                                voxels[cursor.x][cursor.y][cursor.z]->settle_timer = 0;
                                std::swap(voxels[it->x][it->y][it->z], voxels[cursor.x][cursor.y][cursor.z]);
                                moved = true;
                                
                            }

                        }
                    }
                }
            }
            voxels[it->x][it->y][it->z]->checkSettle(moved);
        }

    };

    int chunk::DrawVoxels(float verts[], loc3d curs, int offset) {

        loc3d cursor(0, 0, 0);
        loc2d cursor2(0, 0);

        int indexMax = rdim - 1;
        int currentShell = 0;
        int currentIndex = 0;

        float heightMod;
        int count = 0;

        while (indexMax >= currentShell) {
            cursor.y = currentShell + (location.y * rdim);
            for (int x = 0; x <= currentShell; x++) {
                for (int z = 0; z <= currentShell; z++) {
                    cursor.x = x + (location.x * rdim);
                    cursor.z = z + (location.z * rdim);
                    cursor2 = cursor.collapseZ();
                    currentIndex = (cursor2.x * 3 + cursor2.y * 480) + 28800 + 240;
                    if (curs.x == cursor.x && curs.y == cursor.y && curs.z == cursor.z) {

                        verts[currentIndex] = 0.1f;
                        verts[currentIndex + 1] = 1.0f;
                        verts[currentIndex + 2] = 0.1f;

                        count++;
                    }
                    
                    else if (voxels[x][currentShell][z]->substance->state == "powder" || voxels[x][currentShell][z]->substance->state == "solid") {

                        

                        verts[currentIndex] = (voxels[x][currentShell][z]->substance->color[0] );
                        verts[currentIndex + 1] = (voxels[x][currentShell][z]->substance->color[1] );
                        verts[currentIndex + 2] = (voxels[x][currentShell][z]->substance->color[2] );

                        currentIndex++;
                        count++;
                    
                    }
                    else if (curs.x == cursor.x && curs.y >= cursor.y && curs.z == cursor.z) {

                        verts[currentIndex] += 0.1f;
                        verts[currentIndex + 1] += 0.3f;
                        verts[currentIndex + 2] += 0.1f;

                        count++;
                    }
                    else if (voxels[x][currentShell][z]->substance->state == "liquid" || voxels[x][currentShell][z]->substance->state == "gas") {

                        
                        verts[currentIndex] += voxels[x][currentShell][z]->substance->color[0];// *(voxels[x][currentShell][z]->substance->color[3] * 1.0f) + verts[currentIndex] * (1.0f - voxels[x][currentShell][z]->substance->color[3]);
                        verts[currentIndex + 1] += voxels[x][currentShell][z]->substance->color[1];// *(voxels[x][currentShell][z]->substance->color[3] * 1.0f) + verts[currentIndex] * (1.0f - voxels[x][currentShell][z]->substance->color[3]);
                        verts[currentIndex + 2] += voxels[x][currentShell][z]->substance->color[2];// *(voxels[x][currentShell][z]->substance->color[3] * 1.0f) + verts[currentIndex] * (1.0f - voxels[x][currentShell][z]->substance->color[3]);
                        count++;

                    }
                }
            }
            cursor.x = currentShell + (location.x * rdim);
            for (int y = 0; y <= currentShell - 1; y++) {
                for (int z = 0; z <= currentShell; z++) {
                    cursor.y = y + (location.y * rdim);
                    cursor.z = z + (location.z * rdim);
                    cursor2 = cursor.collapseZ();
                    currentIndex = (cursor2.x * 3 + cursor2.y * 480) + 28800 + 240;
                    if (curs.x == cursor.x && curs.y == cursor.y && curs.z == cursor.z) {

                        verts[currentIndex] = 0.1f;
                        verts[currentIndex + 1] = 1.0f;
                        verts[currentIndex + 2] = 0.1f;

                        count++;
                    }
                    
                    else if (voxels[currentShell][y][z]->substance->state == "powder" || voxels[currentShell][y][z]->substance->state == "solid") {

                        

                        verts[currentIndex] = (voxels[currentShell][y][z]->substance->color[0] );
                        verts[currentIndex + 1] = (voxels[currentShell][y][z]->substance->color[1] );
                        verts[currentIndex + 2] = (voxels[currentShell][y][z]->substance->color[2] );

                        currentIndex++;
                        count++;

                    }
                    else if (curs.x == cursor.x && curs.y >= cursor.y && curs.z == cursor.z) {

                        verts[currentIndex] += 0.1f;
                        verts[currentIndex + 1] += 0.3f;
                        verts[currentIndex + 2] += 0.1f;

                        count++;
                    }
                    else if (voxels[currentShell][y][z]->substance->state == "liquid" || voxels[currentShell][y][z]->substance->state == "gas") {
    

                        verts[currentIndex] += voxels[currentShell][y][z]->substance->color[0];
                        verts[currentIndex + 1] += (voxels[currentShell][y][z]->substance->color[1]);
                        verts[currentIndex + 2] += (voxels[currentShell][y][z]->substance->color[2]);
                        count++;

                    }

                }
            }
            cursor.z = currentShell + (location.z * rdim);
            for (int y = 0; y <= currentShell - 1; y++) {
                for (int x = 0; x <= currentShell - 1; x++) {
                    cursor.y = y + (location.y * rdim);
                    cursor.x = x + (location.x * rdim);
                    cursor2 = cursor.collapseZ();
                    currentIndex = (cursor2.x * 3 + cursor2.y * 480)  + 28800 + 240;

                    if (curs.x == cursor.x && curs.y == cursor.y && curs.z == cursor.z) {
                        
                        verts[currentIndex] = 0.1f;
                        verts[currentIndex + 1] = 1.0f;
                        verts[currentIndex + 2] = 0.1f;

                        count++;
                    }
                    
                    else if (voxels[x][y][currentShell]->substance->state == "powder" || voxels[x][y][currentShell]->substance->state == "solid") {
                        
                        

                        verts[currentIndex] = (voxels[x][y][currentShell]->substance->color[0] );

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][y][currentShell]->substance->color[1] );

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][y][currentShell]->substance->color[2] );

                        currentIndex++;
                        count++;

                    }
                    else if (curs.x == cursor.x && curs.y >= cursor.y && curs.z == cursor.z) {

                        verts[currentIndex] += 0.1f;
                        verts[currentIndex + 1] += 0.3f;
                        verts[currentIndex + 2] += 0.1f;

                        count++;
                    }
                    else if (voxels[x][y][currentShell]->substance->state == "liquid" || voxels[x][y][currentShell]->substance->state == "gas") {

                        

                        verts[currentIndex] += voxels[x][y][currentShell]->substance->color[0];
                        verts[currentIndex + 1] += (voxels[x][y][currentShell]->substance->color[1]);
                        verts[currentIndex + 2] += (voxels[x][y][currentShell]->substance->color[2]);
                        count++;

                    }

                }
            }
            currentShell++;

        }
        return count;
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

