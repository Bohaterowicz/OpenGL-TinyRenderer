#pragma once
#include "glimg_c.h"


namespace glimg{
    namespace util{

        inline uint32 TruncateUInt64ToUInt32(uint64 Value){
            GLIMGAssert(Value <= 0xFFFFFFFF);
            uint32 Result = (uint32)Value;
            return Result;
        }

        inline uint32 BigToSmallEndian(uint32 Val){
            union {
                uint32 u32;
                uint8 u8[4];
            }x = {Val};
            return 
                ((uint32_t)x.u8[0] << 24) |
                ((uint32_t)x.u8[1] << 16) |
                ((uint32_t)x.u8[2] <<  8) |
                            x.u8[3];
        }
    }
}