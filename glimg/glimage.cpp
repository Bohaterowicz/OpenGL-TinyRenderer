#include "glimage.h"
#include "glimg_file.h"
#include "glpng_loader.h"

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

    image::~image(){

    }
}