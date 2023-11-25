#pragma once

#include "includes.h"

namespace Game {

    //LOAD BMP TO ARRAY
//loadBitmap takes in the filespath, pixel array to modify them 
    bool loadBitmap(const std::string& filename, float* pixels, int width, int height) {
        std::ifstream file(filename, std::ios::binary);


        if (!file.is_open()) {
            std::cout << "Failed to open the file: " << filename << std::endl;
            return false;
        }

        // Read the bitmap file header
        char header[54];
        file.read(header, 54);

        // Extract width and height from the header
        int fileWidth = *(int*)&header[18];
        int fileHeight = *(int*)&header[22];
        bool alpha = *(bool*)&header[32];

        if (fileWidth != width || fileHeight != height) {
            std::cout << "Image dimensions do not match the specified width and height." << std::endl;
            file.close();
            return false;
        }

        // Calculate the row size in bytes (including padding)
        int rowSize = ((fileWidth * 3 + 3) & (~3));

        // Allocate memory for the pixel data
        unsigned char* rowBuffer = new unsigned char[rowSize];
        float* pixelPtr = pixels;

        // Read pixel data row by row
        for (int y = 0; y < fileHeight; y++) {
            file.read(reinterpret_cast<char*>(rowBuffer), rowSize);
            int indexImage = 0;
            for (int x = 0; x < fileWidth; x++) {



                int indexPix = x * 3;

                if (alpha) {// Alpha
                    // Normalize the pixel values to the range [0, 1]
                    indexImage = x * 4;
                    pixelPtr[indexPix + 0] = ((static_cast<float>(rowBuffer[indexImage + 3])) / 255.0f);// *static_cast<long float>(rowBuffer[indexImage + 3]) / 255.0f;  // Red
                    pixelPtr[indexPix + 1] = ((static_cast<float>(rowBuffer[indexImage + 2])) / 255.0f);// *static_cast<long float>(rowBuffer[indexImage + 3]) / 255.0f;  // Green
                    pixelPtr[indexPix + 2] = ((static_cast<float>(rowBuffer[indexImage + 1])) / 255.0f);// *static_cast<long float>(rowBuffer[indexImage + 3]) / 255.0f;  // Blue
                }
                else {
                    indexImage = x * 3;
                    pixelPtr[indexPix + 0] = ((static_cast<float>(rowBuffer[indexImage + 2])) / 255.0f);;  // Red
                    pixelPtr[indexPix + 1] = ((static_cast<float>(rowBuffer[indexImage + 1])) / 255.0f);;  // Green
                    pixelPtr[indexPix + 2] = ((static_cast<float>(rowBuffer[indexImage + 0])) / 255.0f);;  // Blue

                }
            }

            pixelPtr += fileWidth * 3;
        }

        // Clean up the allocated memory
        delete[] rowBuffer;

        // Close the file
        file.close();

        return true;
    }

		
		
		
		
		
		
};

