#include "glbmp_loader.h"
#include <cstring>
#include <stdlib.h>

namespace glimg
{
    namespace loader
    {
        bmp_loader_result BMPLoader(void *BMPData, size Size)
        {
            bmp_loader_result Result = {};
            //cast pointer to byte pointer
            uint8 *Data = static_cast<uint8 *>(BMPData);
            //Get the bmp file header...
            bmp_file_header BMPHeader = *(reinterpret_cast<bmp_file_header *>(Data));
            //Get pointer to pixel data;
            uint8 *PixelDataPtr = Data + BMPHeader.DataOffset;
            //Advance data pointer past header struct
            Data += sizeof(bmp_file_header);
            //The type of info header is determined by its size (first 4 bytes in the struct)
            uint32 InfoHeaderSize = *(reinterpret_cast<uint32 *>(Data));
            //We copy the header data into our header, unsued parameters will be 0;
            std::memcpy(&Result.BMPHeader, Data, InfoHeaderSize);

            if (Result.BMPHeader.ImageBufferSize > 0)
            {
                //We allocate memory neded for our pixel buffer
                Result.PixelData = malloc(Result.BMPHeader.ImageBufferSize);
                //We copy pixel data from file to our pixel buffer;
                std::memcpy(Result.PixelData, PixelDataPtr, Result.BMPHeader.ImageBufferSize);
            }

            return Result;
        }
    }
}