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

    class image{
        private:

        public:
        image(std::string Path);
        ~image();

        private:
        image_type CheckImageType(void* RawImageData);
        bool32 IsPNG(void* RawImageData);
    };


}
