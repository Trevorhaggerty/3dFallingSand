#pragma once
#include "includes.h"

namespace Game
{

    struct loc2d {
        loc2d(float xn, float yn) { x = xn; y = yn; };
        float x;
        float y;
        loc2d operator +(loc2d& obj) {
            loc2d result(0.0f, 0.0f);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            return (result); //return object
        };
        loc2d operator -(loc2d& obj) {
            loc2d result(0.0f, 0.0f);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            return (result); //return object
        };
        loc2d operator * (loc2d& obj) {
            loc2d result(0.0f, 0.0f);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            return (result); //return object
        };
        loc2d operator / (loc2d& obj) {
            loc2d result(0.0f, 0.0f);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            return (result); //return object
        };
        /*
        std::ostream& operator<<(std::ostream& s, const loc2d& loc2d) {
            return s << "(" << loc2d.x << ", " << loc2d.y << ")";
        };
        */
    };

    struct loc3d {
        loc3d(int xn, int yn, int zn) { x = xn; y = yn; z = zn; };
        int x;
        int y;
        int z;
        loc3d operator +(loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            result.z = this->z + obj.z;
            return (result); //return object
        };
        loc3d operator -(loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            result.z = this->z - obj.z;
            return (result); //return object
        };
        loc3d operator * (loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            result.z = this->z * obj.z;
            return (result); //return object
        };
        loc3d operator / (loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            result.z = this->z / obj.z;
            return (result); //return object
        };

        int mag() {
            int result = 0;
            result += abs(x);
            result += abs(y);
            result += abs(z);
            return result;
        };

        loc2d collapseZ() {
            loc2d result(0, 0);

            result.x = (float)(x - z);
            result.y = (float)(y)-(x + z) / 2.0f;

            return result;
        };
        /*
        std::ostream& operator<<(std::ostream& s, const loc3d& loc3d) {
            return s << "(" << loc3d.x << ", " << loc3d.y << ", " << loc3d.z << ")";
        };
        */
    };

    loc3d indexToCoord(int index, int dim);
}
