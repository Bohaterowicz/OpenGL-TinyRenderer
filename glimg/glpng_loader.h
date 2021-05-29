#pragma once
#include "glimg_c.h"


namespace glimg{
    namespace loader{

        enum class chunk_type : uint32 {
            IHDR = 0x52444849,
            IEND = 0x444e4549,
            IDAT = 0x54414449,
            PLTE = 0x45544c50, 
            sRGB = 0x42475273,
            gAMA = 0x414d4167,
        };
        inline bool32 operator==(const uint32& lhs, const chunk_type& rhs){return (lhs == static_cast<uint32>(rhs));}
        inline bool operator!=(const uint32& lhs, const chunk_type& rhs){return (lhs != static_cast<uint32>(rhs));}

        struct png_loader_result{
            uint32 x;
        };

        struct chunk_ihdr{
            uint32 Width;
            uint32 Height;
            uint8 BitDepth;
            uint8 ColorType;
            uint8 CompressionType;
            uint8 FilterMethod;
            uint8 InterlaceMethod;
        };

        png_loader_result PNGLoader(void* PNGData, size Size);
        uint32 GetChannelCount(uint8 ColorType);
    }
}