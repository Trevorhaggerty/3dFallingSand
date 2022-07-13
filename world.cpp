
#include "world.h"

namespace Game {

    world::world() {
        loc3d  loc3;
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                for (int z = 0; z < 3; z++) {
                    x = x - 3 / 2;
                    y = y - 3 / 2;
                    z = z - 3 / 2;
                    chunks[x + 3 * y + 9 * z].SetLocation(loc3);
                }
            }
        }
    }





};