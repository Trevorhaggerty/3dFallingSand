#pragma once
#include "voxfuncs.h"
namespace Game{

    loc3d indexToCoord(int index, int dim) {
        loc3d result(0, 0, 0);

        result.x = index / (dim * dim);
        result.y = (index / dim) % dim;
        result.z = index % dim;

        return result;
    }


    loc3d findAtHeight(loc2d mouseCurs, int height) {
        loc3d result;

        result.x = height - mouseCurs.y + (mouseCurs.x / 2);
        result.y = height;
        result.z = height - mouseCurs.y - (mouseCurs.x / 2);


        return result;
    }

 

    // Function to rotate a point around another point
    loc3d rotateloc3d(loc3d point, loc3d center, loc3df rot) {
        
        rot.x = rot.x * PI / 180;
        rot.y = rot.y * PI / 180;
        rot.z = rot.z * PI / 180;

        loc3d translatedPoint = point;

        //std::cout << "rotating " << translatedPoint.x <<"," << translatedPoint.y <<","<< translatedPoint.z << std::endl;

        translatedPoint = point - center;
        //std::cout << "translated to origin " << translatedPoint.x << "," << translatedPoint.y << "," << translatedPoint.z << std::endl;

        // Apply rotation around X-axis
        float cosX = cos(rot.x);
        float sinX = sin(rot.x);
        loc3d rotatedX;
        rotatedX.x = translatedPoint.x;
        rotatedX.y = translatedPoint.y * cosX - translatedPoint.z * sinX;
        rotatedX.z = translatedPoint.y * sinX + translatedPoint.z * cosX;
        //std::cout << "x rotation " << rotatedX.x << "," << rotatedX.y << "," << rotatedX.z << std::endl;

        // Apply rotation around Y-axis
        float cosY = cos(rot.y);
        float sinY = sin(rot.y);
        loc3d rotatedY;
        rotatedY.x = rotatedX.x * cosY + rotatedX.z * sinY;
        rotatedY.y = rotatedX.y;
        rotatedY.z = -rotatedX.x * sinY + rotatedX.z * cosY;
        //std::cout << "y rotation" << rotatedY.x << "," << rotatedY.y << "," << rotatedY.z << std::endl;
        
        // Apply rotation around Z-axis
        float cosZ = cos(rot.z);
        double sinZ = sin(rot.z);
        loc3d rotatedZ;
        rotatedZ.x = rotatedY.x * cosZ - rotatedY.y * sinZ;
        rotatedZ.y = rotatedY.x * sinZ + rotatedY.y * cosZ;
        rotatedZ.z = rotatedY.z;
        //std::cout << "z rotation " << rotatedZ.x << "," << rotatedZ.y << "," << rotatedZ.z << std::endl;
        
        // Translate the point back to its original position
        loc3d transformedPoint;
 
         transformedPoint = rotatedZ + center;
         //std::cout << "result " << transformedPoint.x << "," << transformedPoint.y << "," << transformedPoint.z << std::endl;
         
         return transformedPoint;
    
    }

    loc2d itoloc2d(int index, int xdim, int ydim) {

        loc2d result(0, 0);
        if (index >= 0 || index < xdim * ydim) {
            result.x = index % (xdim)-xdim / 2;
            result.y = index / xdim - ydim / 2;
        }
        return result;
    }

    //distance test for 3d locations
    float distance3d(const loc3d& point1, const loc3d& point2) {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        float dz = point2.z - point1.z;

        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }



    //2d interpolationn returns a location
    loc2d lerp2d(loc2d start, loc2d end, float step) {
        loc2d result;
        result.x = lerp(start.x, end.x, step);
        result.y = lerp(start.y, end.y, step);
        return result;
    }

    //3d interpolation function, returns a location
    loc3d lerp3d(loc3d start, loc3d end, float step) {
        loc3d result;
        result.x = lerp(start.x, end.x, step);
        result.y = lerp(start.y, end.y, step);
        result.z = lerp(start.z, end.z, step);
        return result;
    }
    
    //interpolation function, returns a value
    float lerp(float start, float end, float step) {
        return start + step * (end - start);
    }

}


