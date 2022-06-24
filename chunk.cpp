#pragma once

#include "chunk.h"


namespace Game {

    

    int randomInt(int i) { return std::rand() % i; }

    chunk::chunk() {
        float basecolor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        substance* air = new substance("air", NULL, NULL, -INF, INF, basecolor, 0.001f, "gas");
        voxel airVox(air, 75.0f, true);
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                for (int k = 0; k < 12; k++) {
                    voxels[i][j][k] = airVox;
                }
            }
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

        for (int x = 0; x < 12; x++) {
            for (int y = 0; y < 12; y++) {
                for (int z = 0; z < 12; z++) {
                    if (voxels[x][y][z].settled == false) {
                        unsettled.push_back(loc3d(x, y, z));
                    }
                }
            }
        }


        std::random_shuffle(unsettled.begin(), unsettled.end(), randomInt);

        for (std::vector<loc3d>::iterator it = unsettled.begin(); it != unsettled.end(); ++it) {
            //cout << *it;

            moved = false;

            if (voxels[it->x][it->y][it->z].substance->state == "powder"
                || voxels[it->x][it->y][it->z].substance->state == "liquid") {
                if (it->y >= 0) {
                    if (it->y > 0) {
                        if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x][it->y - 1][it->z].substance->density) {
                            voxels[it->x][it->y - 1][it->z].settled = false;
                            std::swap(voxels[it->x][it->y][it->z], voxels[it->x][it->y - 1][it->z]);
                            moved = true;
                        }
                        else {
                            available.clear();
                            available.shrink_to_fit();

                            negxlim = (it->x <= 0);
                            posxlim = (it->x >= 12 - 1);
                            negzlim = (it->z <= 0);
                            poszlim = (it->z >= 12 - 1);

                            //loop through the bottom 9 (clipping based on edges)
                            if (!negxlim && !negzlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x - 1][it->y - 1][it->z - 1].substance->density) {
                                    available.push_back(loc3d(it->x - 1, it->y - 1, it->z - 1));
                                }
                            }
                            if (!negxlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x - 1][it->y - 1][it->z].substance->density) {
                                    available.push_back(loc3d(it->x - 1, it->y - 1, it->z));
                                }
                            }
                            if (!negxlim && !poszlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x - 1][it->y - 1][it->z + 1].substance->density) {
                                    available.push_back(loc3d(it->x - 1, it->y - 1, it->z + 1));
                                }
                            }

                            if (!posxlim && !negzlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x + 1][it->y - 1][it->z - 1].substance->density) {
                                    available.push_back(loc3d(it->x + 1, it->y - 1, it->z - 1));
                                }
                            }
                            if (!posxlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x + 1][it->y - 1][it->z].substance->density) {
                                    available.push_back(loc3d(it->x + 1, it->y - 1, it->z));
                                }
                            }
                            if (!posxlim && !poszlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x + 1][it->y - 1][it->z + 1].substance->density) {
                                    available.push_back(loc3d(it->x + 1, it->y - 1, it->z + 1));
                                }
                            }
                            if (!negzlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x][it->y - 1][it->z - 1].substance->density) {
                                    available.push_back(loc3d(it->x, it->y - 1, it->z - 1));
                                }
                            }
                            if (!poszlim) {
                                if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x][it->y - 1][it->z + 1].substance->density) {
                                    available.push_back(loc3d(it->x, it->y - 1, it->z + 1));
                                }
                            }

                            if (available.size() == 0 && voxels[it->x][it->y][it->z].substance->state == "liquid") {
                                if (!negxlim && !negzlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x - 1][it->y][it->z - 1].substance->density) {
                                        available.push_back(loc3d(it->x - 1, it->y, it->z - 1));
                                    }
                                }
                                if (!negxlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x - 1][it->y][it->z].substance->density) {
                                        available.push_back(loc3d(it->x - 1, it->y, it->z));
                                    }
                                }
                                if (!negxlim && !poszlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x - 1][it->y][it->z + 1].substance->density) {
                                        available.push_back(loc3d(it->x - 1, it->y, it->z + 1));
                                    }
                                }

                                if (!posxlim && !negzlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x + 1][it->y][it->z - 1].substance->density) {
                                        available.push_back(loc3d(it->x + 1, it->y, it->z - 1));
                                    }
                                }
                                if (!posxlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x + 1][it->y][it->z].substance->density) {
                                        available.push_back(loc3d(it->x + 1, it->y, it->z));
                                    }
                                }
                                if (!posxlim && !poszlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x + 1][it->y][it->z + 1].substance->density) {
                                        available.push_back(loc3d(it->x + 1, it->y, it->z + 1));
                                    }
                                }
                                if (!negzlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x][it->y][it->z - 1].substance->density) {
                                        available.push_back(loc3d(it->x, it->y, it->z - 1));
                                    }
                                }
                                if (!poszlim) {
                                    if (voxels[it->x][it->y][it->z].substance->density > voxels[it->x][it->y][it->z + 1].substance->density) {
                                        available.push_back(loc3d(it->x, it->y, it->z + 1));
                                    }
                                }
                            }
                            if (available.size() != 0) {
                                std::random_shuffle(available.begin(), available.end(), randomInt);
                                cursor = available.front();
                                std::swap(voxels[it->x][it->y][it->z], voxels[cursor.x][cursor.y][cursor.z]);
                                moved = true;
                                voxels[cursor.x][cursor.y][cursor.z].settled = false;
                            }

                        }
                    }
                }
            }
            voxels[it->x][it->y][it->z].checkSettle(moved);
        }

    };

    void chunk::DrawVoxels(float verts[]) {

        loc3d cursor(0, 0, 0);
        loc2d cursor2(0, 0);

        int indexMax = 12 - 1;
        int currentShell = 0;
        int currentIndex = 0;

        float heightMod;


        while (indexMax >= currentShell) {
            cursor.y = currentShell;
            for (int x = 0; x <= currentShell; x++) {
                for (int z = 0; z <= currentShell; z++) {
                    cursor.x = x;
                    cursor.z = z;
                    if (voxels[x][cursor.y][z].substance->name != "air") {
                        cursor2 = cursor.collapseZ();

                        heightMod = 1.0f - cos(PI * cursor.y / 12) / 2;

                        verts[currentIndex] = (float)((cursor2.x) / (SCR_WIDTH / 8));

                        currentIndex++;
                        verts[currentIndex] = (float)((cursor2.y) / (SCR_HEIGHT / 8));

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][cursor.y][z].substance->color[0] * heightMod);

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][cursor.y][z].substance->color[1] * heightMod);

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][cursor.y][z].substance->color[2] * heightMod);

                        currentIndex++;
                    }

                }
            }
            cursor.x = currentShell;
            for (int y = 0; y <= currentShell - 1; y++) {
                for (int z = 0; z <= currentShell; z++) {
                    cursor.y = y;
                    cursor.z = z;
                    if (voxels[cursor.x][y][z].substance->name != "air") {
                        cursor2 = cursor.collapseZ();

                        heightMod = 1.0f - cos(PI * y / 12) / 2;
                        verts[currentIndex] = (float)((cursor2.x) / (SCR_WIDTH / 8));

                        currentIndex++;
                        verts[currentIndex] = (float)((cursor2.y) / (SCR_HEIGHT / 8));

                        currentIndex++;
                        verts[currentIndex] = (voxels[cursor.x][y][z].substance->color[0] * heightMod);

                        currentIndex++;
                        verts[currentIndex] = (voxels[cursor.x][y][z].substance->color[1] * heightMod);

                        currentIndex++;
                        verts[currentIndex] = (voxels[cursor.x][y][z].substance->color[2] * heightMod);

                        currentIndex++;
                    }
                }
            }
            cursor.z = currentShell;
            for (int y = 0; y <= currentShell - 1; y++) {
                for (int x = 0; x <= currentShell - 1; x++) {
                    cursor.y = y;
                    cursor.x = x;
                    if (voxels[x][y][cursor.z].substance->name != "air") {
                        cursor2 = cursor.collapseZ();

                        heightMod = 1.0f - cos(PI * y / 12) / 2;

                        verts[currentIndex] = (float)((cursor2.x) / (SCR_WIDTH / 8));

                        currentIndex++;
                        verts[currentIndex] = (float)((cursor2.y) / (SCR_HEIGHT / 8));

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][y][cursor.z].substance->color[0] * heightMod);

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][y][cursor.z].substance->color[1] * heightMod);

                        currentIndex++;
                        verts[currentIndex] = (voxels[x][y][cursor.z].substance->color[2] * heightMod);

                        currentIndex++;
                    }
                }
            }
            currentShell++;

        }

    };


    void chunk::Save(std::string saveloc, int x, int y, int z) {
    
        saveloc = saveloc + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ".chunk";
            std::fstream chunkFile(saveloc, std::ios::out|std::ios::trunc);
            if (chunkFile.is_open()) {

                for (int x = 0; x < 12; x++) {
                    for (int y = 11; y >= 0; y--) {
                        for (int z = 0; z < 12; z++) {
                            chunkFile << voxels[x][y][z];
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

            for (int x = 0; x < 12; x++) {
                for (int y = 0; y < 12; y++) {
                    for (int z = 0; z < 12; z++) {
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

