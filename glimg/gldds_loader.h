#pragma once
#include "glimg_c.h"
#include "enum_types.h"

#define DDS_HEADER_RESERVED_BYTES_11 11
#define SIGNATURE_BYTE_SIZE 4

#define DXT1_BLOCKSIZE 8
#define DXT3_BLOCKSIZE 16
#define DXT5_BLOCKSIZE 16

namespace glimg
{
    namespace loader
    {
        struct dds_pixelformat
        {
            uint32 Size;
            uint32 Flags;
            uint32 FourCC;
            uint32 RGBBitCount;
            uint32 RBitMask;
            uint32 GBitMask;
            uint32 BBitMask;
            uint32 ABitMask;
        };

        struct dds_header
        {
            uint32 Size;
            uint32 Flags;
            uint32 Height;
            uint32 Width;
            uint32 PitchOrLinearSize;
            uint32 Depth;
            uint32 MipMapCount;
            uint32 Reserved1[DDS_HEADER_RESERVED_BYTES_11];
            dds_pixelformat PixelFormat;
            uint32 Caps;
            uint32 Caps2;
            uint32 Caps3;
            uint32 Caps4;
            uint32 Reserved2;
        };

        struct dds_loader_result
        {
            void *PixelData;
            dds_header ImageInfoHeader;
            compression_format CompressionFormat;
            uint32 BlockSize;
            uint32 ImageBufferSize;
        };

        dds_loader_result DDSLoader(void *DDSData, size Size);
        compression_format GetCompressionFormat(dds_pixelformat &PixelFormat, uint32 *BlockSize);
    }
}