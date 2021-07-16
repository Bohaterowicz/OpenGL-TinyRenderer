#include "glimage.h"
#include "glimg_file.h"
#include "glpng_loader.h"
#include "glbmp_loader.h"
#include "gldds_loader.h"

namespace glimg
{
    image::image(std::string Path)
    {
        file::loaded_file ImageFile(Path);
        if (ImageFile.GetData() && ImageFile.GetSize())
        {
            image_type ImgType = CheckImageType(ImageFile.GetData());
            Type = ImgType;

            switch (ImgType)
            {
            case image_type::PNG:
            {
                GLIMGAssert(false);
                ///loader::png_loader_result PNGResult = loader::PNGLoader(ImageFile.GetData(), ImageFile.GetSize());
                //InflatePNG(PNGResult);
                //UnfilterPng(...);
            }
            break;
            case image_type::BMP:
            {
                loader::bmp_loader_result BMPResult = loader::BMPLoader(ImageFile.GetData(), ImageFile.GetSize());
                PixelData = BMPResult.PixelData;
                PixelDataInfo.Size = BMPResult.BMPHeader.ImageBufferSize;
                PixelDataInfo.Width = BMPResult.BMPHeader.Width;
                PixelDataInfo.Height = BMPResult.BMPHeader.Height;
                PixelDataInfo.BitsPerPixel = BMPResult.BMPHeader.BitCount;
                PixelDataInfo.ColorSpace = BMPResult.BMPHeader.ColorSpace;
                PixelDataInfo.MipmapCount = 1;
            }
            break;
            case image_type::DDS:
            {
                loader::dds_loader_result DDSResult = loader::DDSLoader(ImageFile.GetData(), ImageFile.GetSize());
                PixelData = DDSResult.PixelData;
                PixelDataInfo.Size = DDSResult.ImageBufferSize;
                PixelDataInfo.Width = DDSResult.ImageInfoHeader.Width;
                PixelDataInfo.Height = DDSResult.ImageInfoHeader.Height;
                PixelDataInfo.BitsPerPixel = DDSResult.ImageInfoHeader.PixelFormat.RGBBitCount;
                PixelDataInfo.CompressionFormat = DDSResult.CompressionFormat;
                PixelDataInfo.BlockSize = DDSResult.BlockSize;
                PixelDataInfo.MipmapCount = DDSResult.ImageInfoHeader.MipMapCount;
            }
            break;
            default:
            {
                //Could not identify image type...
            }
            break;
            }
        }
        else
        {
            //Failed to load image from path
        }
    }

    image_type image::CheckImageType(void *RawImageData)
    {
        image_type Type;
        if (IsPNG(RawImageData) == TRUE)
        {
            Type = image_type::PNG;
        }
        else if (IsBMP(RawImageData) == TRUE)
        {
            Type = image_type::BMP;
        }
        else if (IsDDS(RawImageData) == TRUE)
        {
            Type = image_type::DDS;
        }
        else
        {
            Type = image_type::UNDEFINED;
        }
        return Type;
    }

    bool32 image::IsPNG(void *RawImageData)
    {
        uint64 PNGSignature = 0x0a1a0a0d474e5089;
        uint64 *DataSignature = (uint64 *)RawImageData;
        if (PNGSignature == *DataSignature)
        {
            return TRUE;
        }
        return FALSE;
    }

    bool32 image::IsBMP(void *RawImageData)
    {
        uint16 BMPSignature = 0x4d42;
        uint16 *DataSignature = (uint16 *)RawImageData;
        if (BMPSignature == *DataSignature)
        {
            return TRUE;
        }
        return FALSE;
    }

    bool32 image::IsDDS(void *RawImageData)
    {
        uint32 DDSSignature = 0x20534444;
        uint32 *DataSignature = (uint32 *)RawImageData;
        if (DDSSignature == *DataSignature)
        {
            return TRUE;
        }
        return FALSE;
    }

    image::~image()
    {
        if (PixelData != nullptr)
        {
            free(PixelData);
        }
    }
}