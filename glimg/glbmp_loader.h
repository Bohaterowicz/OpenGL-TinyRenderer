#pragma once
#include "glimg_c.h"

namespace glimg{
    namespace loader{

#pragma pack(push, 1)
        struct bmp_file_header
        {
            uint16 TypeSignature;
            uint32 Size;
            uint16 Reserved1;
            uint16 Reserved2;
            uint32 DataOffset;
        };
#pragma pack(pop)

        struct CIEXYZ{
            int32 X;
            int32 Y;
            int32 Z;
        };

        struct CIEXYZ_triple{
            CIEXYZ Red;
            CIEXYZ Green;
            CIEXYZ Blue;
        };
#pragma pack(push, 1)
        struct bmp_base_info_header{
            uint32 Size;
            int32 Width;
            int32 Height;
            uint16 Planes;
            uint16 BitCount;
            uint32 Compression;
            uint32 ImageBufferSize;
            int32 PixelsPerMeterX;
            int32 PixelsPerMeterY;
            uint32 ColorsUsed;
            uint32 ColorsImportant;
        };
#pragma pack(pop)

        struct bmp_info_v4_header{
            uint32 Size;
            int32 Width;
            int32 Height;
            uint16 Planes;
            uint16 BitCount;
            uint32 Compression;
            uint32 ImageBufferSize;
            int32 PixelsPerMeterX;
            int32 PixelsPerMeterY;
            uint32 ColorsUsed;
            uint32 ColorsImportant;
            uint32 RedMask;
            uint32 GreenMask;
            uint32 BlueMask;
            uint32 AlphaMask;
            uint32 ColorSpace;
            CIEXYZ_triple ColorEndpoints;
            uint32 RedGamma;
            uint32 GreenGamma;
            uint32 BlueGamme;
        };
        
        struct bmp_info_v5_header{
            uint32 Size;
            int32 Width;
            int32 Height;
            uint16 Planes;
            uint16 BitCount;
            uint32 Compression;
            uint32 ImageBufferSize;
            int32 PixelsPerMeterX;
            int32 PixelsPerMeterY;
            uint32 ColorsUsed;
            uint32 ColorsImportant;
            uint32 RedMask;
            uint32 GreenMask;
            uint32 BlueMask;
            uint32 AlphaMask;
            uint32 ColorSpace;
            CIEXYZ_triple ColorEndpoints;
            uint32 RedGamma;
            uint32 GreenGamma;
            uint32 BlueGamme;
            uint32 Intent;
            uint32 ProfileData;
            uint32 ProfileSize;
            uint32 Reserved;
        };
        typedef bmp_info_v5_header bmp_info_header;

        struct bmp_loader_result
        {
            void* PixelData;
            bmp_info_header BMPHeader;
            /* data */
        };
        
        bmp_loader_result BMPLoader(void* BMPData, size Size);
    }
}