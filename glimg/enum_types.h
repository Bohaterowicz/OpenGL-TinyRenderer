#pragma once

namespace glimg
{
    enum class image_type
    {
        PNG,
        JPEG,
        BMP,
        DDS,
        UNDEFINED,
    };

    enum class compression_format
    {
        DXT1,
        DXT3,
        DXT5,
        NONE,
    };
}