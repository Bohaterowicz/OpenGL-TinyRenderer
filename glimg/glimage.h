#pragma once
#include "glimg_c.h"
#include <string>

namespace glimg{

    enum class image_type{
        PNG,
        JPEG,
        BMP,
        DDS,
        UNDEFINED,
    };

    struct pixel_data_info{
        uint32 Size;
        uint32 Width;
        uint32 Height;
        uint16 BitsPerPixel;
        uint32 ColorSpace;
    };

    class image{
        private:
        void* PixelData;
        pixel_data_info PixelDataInfo;

        public:
        image(std::string Path);
        ~image();

        private:
        image_type CheckImageType(void* RawImageData);
        bool32 IsPNG(void* RawImageData);
        bool32 image::IsBMP(void* RawImageData);
    };


}
