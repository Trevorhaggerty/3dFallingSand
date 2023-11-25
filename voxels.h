#pragma once

#include "includes.h"
#include "substance.h"


namespace Game
{
    // voxel as 32 bit int
    // 0000 0000 0000 0000 0000 0000 0000 0000
    // 0000 0000 0000 0000 0000 0000  0000  00   00    0        0
    //      velocity       chemical   timer temp light visible  resting
    

    // 

    struct voxel {
        //constructor functions
        voxel(substance* sub, float t, bool s = true) {
            substance = sub;
            subId = sub->id;
            temperature = t;
            settled = s;
            settle_timer = 0;
            flags = 0;
            //voxelLogger.updateLocation("voxel struct");
            std::string message = "substance:" + sub->name + ", Temp: " + std::to_string(t) + "settled: " + std::to_string(s);
            //voxelLogger.log(1, "voxel made", message);
        }
        voxel() {
            substance = NULL;
            subId = 0;
            temperature = INF;
            settled = true;
            settle_timer = 0;
            flags = 0;
            logger.updateLocation("voxel");
            //voxelLogger.updateLocation("voxel struct");
            //voxelLogger.log(1, "blank voxel made", "");

        }


        //TODO function for comparing 
        void checkChemistry(substance substance1, substance substance2) {
           
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
            if (settle_timer >= 9) {
                settled = true;
            }
            
            
            
        }
        void operator = (const voxel& v) {

            substance = v.substance;
            temperature = v.temperature;
            settled = v.settled;
            subId = v.subId;
            settle_timer = v.settle_timer;
            flags = v.flags;

        };

        short unsigned int flags;
        int subId;
        substance* substance;
        float temperature;
        bool settled;
        int settle_timer;
        friend std::ostream& operator<<(std::ostream& out, const voxel& voxel);
        Logger logger;
    };

  
    

}



