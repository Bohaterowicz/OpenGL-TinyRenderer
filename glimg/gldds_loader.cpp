#include "gldds_loader.h"
#include <cstring>
#include <stdlib.h>

namespace glimg
{
    namespace loader
    {
        dds_loader_result DDSLoader(void *DDSData, size Size)
        {
            dds_loader_result Result = {};

            //Get the byte pointer to the data
            uint8 *Data = static_cast<uint8 *>(DDSData);
            //Advance the pointer past the signature
            Data += SIGNATURE_BYTE_SIZE;

            //Load the header data
            std::memcpy(&Result.ImageInfoHeader, Data, sizeof(dds_header));
            Data += sizeof(dds_header);

            Result.CompressionFormat = GetCompressionFormat(Result.ImageInfoHeader.PixelFormat, &Result.BlockSize);

            size ImageDataSize = Size - (sizeof(dds_header) + SIGNATURE_BYTE_SIZE);
            Result.ImageBufferSize = ImageDataSize;
            Result.PixelData = malloc(ImageDataSize);
            std::memcpy(Result.PixelData, Data, ImageDataSize);

            return Result;
        }

        compression_format GetCompressionFormat(dds_pixelformat &PixelFormat, uint32 *BlockSize)
        {
            uint8 *FourCC = reinterpret_cast<uint8 *>(&PixelFormat.FourCC);

            switch (FourCC[3])
            {
            case '1':
            {
                *BlockSize = DXT1_BLOCKSIZE;
                return compression_format::DXT1;
            }
            break;
            case '3':
            {
                *BlockSize = DXT3_BLOCKSIZE;
                return compression_format::DXT3;
            }
            break;
            case '5':
            {
                *BlockSize = DXT5_BLOCKSIZE;
                return compression_format::DXT5;
            }
            break;
            default:
            {
                return compression_format::NONE;
            }
            break;
            }
        }
    }
}
