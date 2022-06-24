#pragma once

#include "includes.h"
#include "substance.h"
namespace Game
{
    struct voxel {
        voxel(substance* sub, float t, bool s) {
            substance = sub;
            temperature = t;
            settled = s;
            settle_timer = 0;
        }
        voxel() {
            substance = NULL;
            temperature = INF;
            settled = true;
            settle_timer = 0;
        }


        void checkTemp() {
            if (temperature < substance->loThresh) {
                substance = substance->loSub;
            }
            if (temperature > substance->hiThresh) {
                substance = substance->hiSub;
            }
        }
        void checkSettle(bool moved) {
            if (!moved) {
                settle_timer++;

            }
            else {
                settle_timer = 0;
                settled = false;
            }
            if (settle_timer >= 5) {
                settled = true;
            }
        }
        void operator = (const voxel& v) {

            substance = v.substance;
            temperature = v.temperature;
            settled = v.settled;

        };


        substance* substance;
        float temperature;
        bool settled;
        int settle_timer;
        friend std::ostream& operator<<(std::ostream& out, const voxel& voxel);
        
    };


}



