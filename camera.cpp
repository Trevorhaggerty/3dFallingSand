#pragma once

#include "camera.h"


namespace Game {

    //main rendering prep function
    void camera::renderVoxels(region* rgn, float* pixels) {


        logger.log(1, "BEGINING VOX RENDERING LOOP", "Begining the raycasting pixel data collection loop");
        for (int pixIndex = 0; pixIndex < PIXELCOUNT; pixIndex = pixIndex + 1) {
          
            logger.log(1, "NEW VOX RNDR LOOP", "loop #" + std::to_string(pixIndex));

            logger.log(1, "CREATING VARS", "creating variables for tracking");
            loc2d gridpoint(0, 0);
            loc3d startloc(0, 0, 0);
            loc3d endloc(0, 0, 0);
            loc3d cursor(0, 0, 0);
            loc4df pixelinfo(0.0f, 0.0f, 0.0f, 0.0f);
            logger.log(1, "VARS CREATED", "Initialized tracking vars");

            logger.log(1, "STARTLOC DEF", "determining startloc initial position relative to cam location");
            gridpoint = itoloc2d(pixIndex, dimensions.x, dimensions.y);
            startloc.x = gridpoint.x;
            startloc.z = gridpoint.y;
            logger.log(1, "STARTLOC DEF", "the start location: (" + std::to_string(startloc.x)+ "," + std::to_string(startloc.y) + "," + std::to_string(startloc.z)+")");


            logger.log(1, "ENDLOC DEF", "determining endloc initial position relative to cam location");
            endloc.x = startloc.x;
            endloc.y = dimensions.z;
            endloc.x = startloc.x;
            logger.log(1, "ENDLOC DEF", "the start location: (" + std::to_string(endloc.x) + "," + std::to_string(endloc.y) + "," + std::to_string(endloc.z) + ")");


            voxel temp;
            
            logger.log(1, "ROTATING STARTLOC", "rotating (" + std::to_string(startloc.x)+ ", " + std::to_string(startloc.y) + ", " + std::to_string(startloc.z)
                + ") around (" + std::to_string(location.x) + ", " + std::to_string(location.y) + ", " + std::to_string(location.z)
                + ") with the angle (" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z));
            startloc = rotateloc3d(startloc, location, rotation);
            logger.log(1, "STARTLOC DEF", "the start location post rotation: (" + std::to_string(startloc.x) + "," + std::to_string(startloc.y) + "," + std::to_string(startloc.z) + ")");

            logger.log(1, "ROTATING ENDLOC", "rotating (" + std::to_string(endloc.x) + ", " + std::to_string(endloc.y) + ", " + std::to_string(endloc.z)
                + ") around (" + std::to_string(location.x) + ", " + std::to_string(location.y) + ", " + std::to_string(location.z)
                + ") with the angle (" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z));
            endloc = rotateloc3d(endloc, location, rotation);
            logger.log(1, "ENDLOC DEF", "the end  location post rotation: (" + std::to_string(endloc.x) + "," + std::to_string(endloc.y) + "," + std::to_string(endloc.z) + ")");

            bool complete = false;
            float i = 0;
            

            logger.log(1, "", "");


            while(!complete){
                
                cursor = lerp3d(startloc, endloc, i / dimensions.z);

                //temp = rgn->GetVoxel(cursor);

                if (temp.substance != nullptr) {

                    pixelinfo = pixelinfo + (temp.substance->color);
                   ;
                }
                
                i++;
                if (i > dimensions.z || pixelinfo.w >= 1.0f) {
                    complete = true;
                }
            }
                int trueIndex = pixIndex * 3;
                *(pixels + trueIndex) += pixelinfo.x;
                *(pixels + trueIndex + 1) += pixelinfo.y;
                *(pixels + trueIndex + 2) += pixelinfo.z;
        }
    };




}


