#include "glpng_loader.h"

#include "util.h"

#define CHUNK_TYPE(type) (static_cast<uint32>((type)))

namespace glimg{
    namespace loader{

        png_loader_result PNGLoader(void* PNGData, size Size){
            png_loader_result Result = {};
            //PNG operates on chunks, we will use those varaibles to store our current chunk size and type!
            uint32 ChunkSize;
            uint32 ChunkType;
            //Asign pointer to a byte poitner
            uint8* Data = (uint8*)PNGData;
            //Skip the 8 byte signature
            Data += 8;
            //Get first chunk size (in bytes) 
            //IMPORTANT: 4 byte integers in PNG format are big endian, we need to reverse it to small endian!
            ChunkSize = util::BigToSmallEndian(*((uint32*)Data));
            //Advance the pointer past the size, to the chunk type
            Data += 4;
            //Get chunk type.
            //IMPORTANT: This chunk_type must be IHDR!
            ChunkType = *((uint32*)Data);
            if(ChunkType == chunk_type::IHDR){
                //Advance pointer past the chunk type, to the chunk data
                Data += 4;
                //Load the chunk data
                chunk_ihdr IHDR_Chunk = *((chunk_ihdr*)Data);
                //We have to convert 4byte integers from big to small endian again...
                IHDR_Chunk.Width = util::BigToSmallEndian(IHDR_Chunk.Width);
                IHDR_Chunk.Height = util::BigToSmallEndian(IHDR_Chunk.Height);

                //We can now reserve memory needed for the image:
                size PixelDataSize = IHDR_Chunk.Width * IHDR_Chunk.Height * IHDR_Chunk.BitDepth * GetChannelCount(IHDR_Chunk.ColorType); 

                //Advance pointer past the chunk data, to the CRC
                Data += ChunkSize;
                //Advance the poitner past the CRC (we dont need it)
                Data += 4;
                //Now we have to loop over all chunks till we get to the IEND chunk
                while(ChunkType != chunk_type::IEND){
                    //We start the loop by getting the new chunk size and advancing past it to chunk type
                    ChunkSize = util::BigToSmallEndian(*((uint32*)Data));
                    Data += 4;
                    //Get the new chunk type and advance past it
                    ChunkType = *((uint32*)Data);
                    Data  += 4;

                    //We switch trough the relevant chunk types
                    switch (ChunkType){
                        case CHUNK_TYPE(chunk_type::IDAT):{
                            ChunkType = 1;
                        }break;
                        case CHUNK_TYPE(chunk_type::sRGB):{
                            ChunkType = 2;
                        }break;
                        case CHUNK_TYPE(chunk_type::gAMA):{
                            ChunkType = 3;
                        }
                    }

                    //Advance the poitner past the current chunk data
                    Data += ChunkSize;
                    //Advance past CRC...
                    Data += 4;
                }
            }
            else{
                //First chunk was not IHDR, png file possibly corrupted??
            }

            return Result;
        }

        uint32 GetChannelCount(uint8 ColorType){
            if(ColorType == 0 || ColorType == 3){
                return 1;
            }
            else if(ColorType == 2){
                return 3;
            }
            else if(ColorType == 4){
                return 2;
            }
            else if(ColorType == 6){
                return 4;
            }
            return 0;
        }

    }
}