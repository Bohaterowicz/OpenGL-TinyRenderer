#pragma once
#include "glimg_c.h"
#include "enum_types.h"
#include <string>

namespace glimg
{
    struct pixel_data_info
    {
        uint32 Size;
        uint32 Width;
        uint32 Height;
        uint16 BitsPerPixel;
        uint32 ColorSpace;
        compression_format CompressionFormat;
        uint32 BlockSize;
        uint32 MipmapCount;
    };

    class image
    {
    private:
        void *PixelData;
        pixel_data_info PixelDataInfo;
        image_type Type;

    public:
        image(std::string Path);
        ~image();
        inline bool32 LoadSucceeded() const { return (PixelData != nullptr) ? TRUE : FALSE; }
        inline pixel_data_info *GetPixelDataInfo() { return &PixelDataInfo; }
        inline image_type GetType() const { return Type; }
        inline void *GetDataPtr() const { return PixelData; }

    private:
        image_type CheckImageType(void *RawImageData);
        bool32 IsPNG(void *RawImageData);
        bool32 IsBMP(void *RawImageData);
        bool32 IsDDS(void *RawImageData);
    };

}
