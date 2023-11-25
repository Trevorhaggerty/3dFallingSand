#pragma once
//voxfuncs contains structs and functions to manipulate them
#include "includes.h"

namespace Game
{
    //2 dimensional location struct
    struct loc2d {
        loc2d() { x = 0; y = 0; };
        loc2d(int xn, int yn) { x = xn; y = yn; };
        float x;
        float y;
        loc2d operator +(loc2d& obj) {
            loc2d result(0, 0);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            return (result); //return object
        };
        loc2d operator +(int& i) {
            loc2d result(0, 0);
            result.x = this->x + i;
            result.y = this->y + i;
            return (result); //return object
        };
        loc2d operator -(loc2d& obj) {
            loc2d result(0, 0);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            return (result); //return object
        };
        loc2d operator -(int& i) {
            loc2d result(0, 0);
            result.x = this->x - i;
            result.y = this->y - i;
            return (result); //return object
        };
        loc2d operator * (loc2d& obj) {
            loc2d result(0, 0);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            return (result); //return object
        };
        loc2d operator * (int& i) {
            loc2d result(0, 0);
            result.x = this->x * i;
            result.y = this->y * i;
            return (result); //return object
        };
        loc2d operator / (loc2d& obj) {
            loc2d result(0, 0);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            return (result); //return object
        };
        loc2d operator / (int& i) {
            loc2d result(0, 0);
            result.x = this->x / i;
            result.y = this->y / i;
            return (result); //return object
        };
        float mag() {
            float result = 0;
            result = sqrt((x * x) + (y * y));
            return result;
        };
        void norm() {
            float M = mag();
            x /= M;
            y /= M;
        };

    };
    struct loc2df {
        loc2df() { x = 0; y = 0; };
        loc2df(float xn, float yn) { x = xn; y = yn; };
        float x;
        float y;
        loc2df operator +(loc2df& obj) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            return (result); //return object
        };
        loc2df operator +(float& f) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x + f;
            result.y = this->y + f;
            return (result); //return object
        };
        loc2df operator -(loc2df& obj) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            return (result); //return object
        };
        loc2df operator -(float& f) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x - f;
            result.y = this->y - f;
            return (result); //return object
        };
        loc2df operator * (loc2df& obj) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            return (result); //return object
        };
        loc2df operator * (float& f) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x * f;
            result.y = this->y * f;
            return (result); //return object
        };
        loc2df operator / (loc2df& obj) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            return (result); //return object
        };
        loc2df operator / (float& f) {
            loc2df result(0.0f, 0.0f);
            result.x = this->x / f;
            result.y = this->y / f;
            return (result); //return object
        };
        float mag() {
            float result = 0;
            result = sqrt((x * x) + (y * y));
            return result;
        };
        void norm() {
            float M = mag();
            x /= M;
            y /= M;
        };
        
    };

    struct loc3d {
        loc3d() { x = 0; y = 0; z = 0; };
        loc3d(int xn, int yn, int zn) { x = xn; y = yn; z = zn; };
        int x;
        int y;
        int z;
        loc3d operator + (loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            result.z = this->z + obj.z;
            return (result); //return object
        };
        loc3d operator + (float& f) {
            loc3d result(0, 0, 0);
            result.x = this->x + f;
            result.y = this->y + f;
            result.z = this->z + f;
            return (result); //return object
        };
        loc3d operator - (loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            result.z = this->z - obj.z;
            return (result); //return object
        };
        loc3d operator - (float& f) {
            loc3d result(0, 0, 0);
            result.x = this->x - f;
            result.y = this->y - f;
            result.z = this->z - f;
            return (result); //return object
        };
        loc3d operator * (loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            result.z = this->z * obj.z;
            return (result); //return object
        };
        loc3d operator * (float& f) {
            loc3d result(0, 0, 0);
            result.x = this->x * f;
            result.y = this->y * f;
            result.z = this->z * f;
            return (result); //return object
        };
        loc3d operator / (loc3d& obj) {
            loc3d result(0, 0, 0);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            result.z = this->z / obj.z;
            return (result); //return object
        };
        loc3d operator / (int& i) {
            loc3d result(0, 0, 0);
            result.x = this->x / i;
            result.y = this->y / i;
            result.z = this->z / i;
            return (result); //return object
        };

        float mag() {
            float result = 0;
            result = sqrt((x * x) + (y * y) + (z * z));
            return result;
        };
        void norm() {
            float M = mag();
            x /= M;
            y /= M;
            z /= M;
        };

        loc2d collapseZ() {
            loc2d result(0, 0);

            result.x = (float)(x - z);
            result.y = (float)(y)-(x + z) / 2;

            return result;
        };
        
    };

    struct loc3df {
        loc3df() { x = 0; y = 0; z = 0; };
        loc3df(float xn, float yn, float zn) { x = xn; y = yn; z = zn; };
        float x;
        float y;
        float z;

        loc3df operator + (loc3df& obj) {
            loc3df result(0, 0, 0);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            result.z = this->z + obj.z;
            return (result); //return object
        };
        loc3df operator + (float& f) {
            loc3df result(0, 0, 0);
            result.x = this->x + f;
            result.y = this->y + f;
            result.z = this->z + f;
            return (result); //return object
        };
        loc3df operator - (loc3df& obj) {
            loc3df result(0, 0, 0);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            result.z = this->z - obj.z;
            return (result); //return object
        };
        loc3df operator - (float& f) {
            loc3df result(0, 0, 0);
            result.x = this->x - f;
            result.y = this->y - f;
            result.z = this->z - f;
            return (result); //return object
        };
        loc3df operator * (loc3df& obj) {
            loc3df result(0, 0, 0);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            result.z = this->z * obj.z;
            return (result); //return object
        };
        loc3df operator * (float& f) {
            loc3df result(0, 0, 0);
            result.x = this->x * f;
            result.y = this->y * f;
            result.z = this->z * f;
            return (result); //return object
        };
        loc3df operator / (loc3df& obj) {
            loc3df result(0, 0, 0);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            result.z = this->z / obj.z;
            return (result); //return object
        };

        float mag() {
            float result = 0;
            result = sqrt((x * x) + (y * y) + (z * z));
            return result;
        };

        void norm() {
            float M = mag();
            x /= M;
            y /= M;
            z /= M;
        };


        loc2df collapseZ() {
            loc2df result(0, 0);

            result.x = (float)(x - z);
            result.y = (float)(y)-(x + z) / 2;

            return result;
        };
        
    };


    struct loc4d {
        loc4d() { x = 0; y = 0; z = 0; w = 0; };
        loc4d(int xn, int yn, int zn, int wn) { x = xn; y = yn; z = zn; w = wn; };
        int x;
        int y;
        int z;
        int w;
        loc4d operator + (loc4d& obj) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            result.z = this->z + obj.z;
            result.w = this->w + obj.w;
            return result; //return object
        };
        loc4d operator + (int& i) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x + i;
            result.y = this->y + i;
            result.z = this->z + i;
            result.w = this->w + i;
            return result; //return object
        };
        loc4d operator - (loc4d& obj) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            result.z = this->z - obj.z;
            result.w = this->w - obj.w;
            return (result); //return object
        };
        loc4d operator - (int& i) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x - i;
            result.y = this->y - i;
            result.z = this->z - i;
            result.w = this->w - i;
            return (result); //return object
        };
        loc4d operator * (loc4d& obj) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            result.z = this->z * obj.z;
            result.w = this->w * obj.w;
            return (result); //return object
        };
        loc4d operator * (float& f) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x * f;
            result.y = this->y * f;
            result.z = this->z * f;
            result.w = this->w * f;
            return (result); //return object
        };
        loc4d operator / (loc4d& obj) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            result.z = this->z / obj.z;
            result.w = this->w / obj.w;
            return (result); //return object
        };
        loc4d operator / (float& f) {
            loc4d result(0, 0, 0, 0);
            result.x = this->x / f;
            result.y = this->y / f;
            result.z = this->z / f;
            result.w = this->w / f;
            return (result); //return object
        };
        
      
    };

    struct loc4df {
        loc4df() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; };
        loc4df(float xn, float yn, float zn, float wn) { x = xn; y = yn; z = zn; w = wn; };
        float x;
        float y;
        float z;
        float w;
        
        loc4df operator + (loc4df& obj) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x + obj.x;
            result.y = this->y + obj.y;
            result.z = this->z + obj.z;
            result.w = this->w + obj.w;
            return (result); //return object
        };

        loc4df operator + (float& f) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x + f;
            result.y = this->y + f;
            result.z = this->z + f;
            result.w = this->w + f;
            return (result); //return object
        };

        loc4df operator - (loc4df& obj) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x - obj.x;
            result.y = this->y - obj.y;
            result.z = this->z - obj.z;
            result.w = this->w - obj.w;
            return (result); //return object
        };

        loc4df operator - (float& f) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x - f;
            result.y = this->y - f;
            result.z = this->z - f;
            result.w = this->w - f;
            return (result); //return object
        };

        loc4df operator * (loc4df& obj) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x * obj.x;
            result.y = this->y * obj.y;
            result.z = this->z * obj.z;
            result.w = this->w * obj.w;
            return (result); //return object
        };

        loc4df operator * (float& f) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x * f;
            result.y = this->y * f;
            result.z = this->z * f;
            result.w = this->w * f;
            return (result); //return object
        };

        loc4df operator / (loc4df& obj) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x / obj.x;
            result.y = this->y / obj.y;
            result.z = this->z / obj.z;
            result.w = this->w / obj.w;
            return (result); //return object
        };
        
        loc4df operator / (float& f) {
            loc4df result(0.0f, 0.0f, 0.0f, 0.0f);
            result.x = this->x / f;
            result.y = this->y / f;
            result.z = this->z / f;
            result.w = this->w / f;
            return (result); //return object
        };


       
    };


    loc3d findAtHeight(loc2d mouseCurs, int height);

    loc3d rotateloc3d(loc3d point, loc3d center, loc3df rot);

    loc2d itoloc2d(int index, int xdim, int ydim);
  
    //distance test for 3d locations
    float distance3d(const loc3d& point1, const loc3d& point2);

    //2d interpolationn returns a location
    loc2d lerp2d(loc2d start, loc2d end, float step);

    //3d interpolation function, returns a location
    loc3d lerp3d(loc3d start, loc3d end, float step);
    
    //interpolation function, returns a value
    float lerp(float start, float end, float step);
    



}
