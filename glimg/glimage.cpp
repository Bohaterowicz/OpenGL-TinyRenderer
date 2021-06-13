#include "glimage.h"
#include "glimg_file.h"
#include "glpng_loader.h"
#include "glbmp_loader.h"

namespace glimg{
    image::image(std::string Path){
        file::loaded_file ImageFile(Path);
        if(ImageFile.GetData() && ImageFile.GetSize()){
            image_type ImgType = CheckImageType(ImageFile.GetData());
            
            switch(ImgType){
                case image_type::PNG:{
                    loader::png_loader_result PNGResult = loader::PNGLoader(ImageFile.GetData(), ImageFile.GetSize());
                    //ProcessPNG(PNGResult);
                }break;
                case image_type::BMP:{
                    loader::bmp_loader_result BMPResult = loader::BMPLoader(ImageFile.GetData(), ImageFile.GetSize());
                    PixelData = BMPResult.PixelData;
                    PixelDataInfo.Size = BMPResult.BMPHeader.ImageBufferSize;
                    PixelDataInfo.Width = BMPResult.BMPHeader.Width;
                    PixelDataInfo.Height = BMPResult.BMPHeader.Height;
                    PixelDataInfo.BitsPerPixel = BMPResult.BMPHeader.BitCount;
                    PixelDataInfo.ColorSpace = BMPResult.BMPHeader.ColorSpace;
                }break;
                default:{
                    //Could not identify image type...
                }break;
            }
        }
        else{
            //Failed to load image from path
        }
    }

    image_type image::CheckImageType(void* RawImageData){
        if(IsPNG(RawImageData)){
            return image_type::PNG;
        }
        else if(IsBMP(RawImageData)){
            return image_type::BMP;
        }
        else{
            return image_type::UNDEFINED;
        }
    }

    bool32 image::IsPNG(void* RawImageData){
        uint64 PNGSignature = 0x0a1a0a0d474e5089;
        uint64* DataSignature = (uint64*)RawImageData;
        if(PNGSignature == *DataSignature){
            return true;
        }
        return false;
    }

    bool32 image::IsBMP(void* RawImageData){
        uint16 BMPSignature = 0x4d42;
        uint16* DataSignature = (uint16*)RawImageData;
        if(BMPSignature == *DataSignature){
            return true;
        }
        return false;
    }

    image::~image(){

    }
}