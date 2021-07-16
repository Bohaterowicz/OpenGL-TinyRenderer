#include "glpng_loader.h"

#include "util.h"

#include<vector>
#include<cstring>

#define CHUNK_TYPE_UINT(type) (static_cast<uint32>((type)))

namespace glimg{
    namespace loader{

        png_loader_result PNGLoader(void* PNGData, size Size){
            png_loader_result Result = {};
            //PNG operates on chunks, we will use those variables to store our current chunk data size and type!
            uint32 ChunkDataSize;
            uint32 ChunkType;
            //Assign pointer to a byte poitner
            uint8* Data = (uint8*)PNGData;
            //Skip the 8 byte signature, we already checked it before
            Data += 8;
            //Get first chunk size (in bytes) 
            //IMPORTANT: 4 byte integers in PNG format are big endian, we need to reverse it to small endian!
            ChunkDataSize = util::BigToSmallEndian(*((uint32*)Data));
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
                //size PixelDataSize = IHDR_Chunk.Width * IHDR_Chunk.Height * IHDR_Chunk.BitDepth * GetChannelCount(IHDR_Chunk.ColorType); 

                //Advance pointer past the chunk data, to the CRC
                Data += ChunkDataSize;
                //Advance the poitner past the CRC (we dont need it)
                Data += 4;

                //We create vector that will store the pointers to data in data chunks (and their data size), since png can split the data into multiple data chunks.
                //We need to have all data in one continous buffer in order to decompress.
                std::vector<data_chunk_info> ChunkDataInfoVec;
                //We also want to store the total size of all data, so we can allocate needed amount of memory as buffer
                uint32 BufferSize = 0;
                //Now we have to loop over all chunks till we get to the IEND chunk
                while(ChunkType != chunk_type::IEND){
                    //We start the loop by getting the new chunk size and advancing past it to chunk type
                    ChunkDataSize = util::BigToSmallEndian(*((uint32*)Data));
                    Data += 4;
                    //Get the new chunk type and advance past it to data
                    ChunkType = *((uint32*)Data);
                    Data  += 4;

                    //We switch trough the relevant chunk types
                    switch (ChunkType){
                        case CHUNK_TYPE_UINT(chunk_type::IDAT):{
                            //We store data chunk info that we nned to create continous buffer for our data
                            BufferSize += ChunkDataSize;
                            data_chunk_info Info = {Data, ChunkDataSize};
                            ChunkDataInfoVec.push_back(Info);
                        }break;
                        case CHUNK_TYPE_UINT(chunk_type::sRGB):{
                            ChunkType = 2;
                        }break;
                        case CHUNK_TYPE_UINT(chunk_type::gAMA):{
                            ChunkType = 3;
                        }
                    }
                    //TODO: Those two could be put together...
                    //Advance the poitner past the current chunk data
                    Data += ChunkDataSize;
                    //Advance past CRC...
                    Data += 4;
                }
                //check that we got some data...
                if(ChunkDataInfoVec.size() > 0 && BufferSize > 0){
                    //Now we allocate buffer needed to store our data
                    uint8* DataBuffer = new uint8[BufferSize];
                    //We copy the data from the individual chunks into our buffer
                    uint32 BufferOffset = 0;
                    for(auto& Info : ChunkDataInfoVec){
                        std::memcpy(DataBuffer + BufferOffset, Info.DataPtr, Info.DataSize);
                        BufferOffset += Info.DataSize;
                    }
                    //We now should have a continous buffer with all (compressed) image data, we can now decompress it using INFLATE algorithm
                    decompressed_data PixelData = DecompressData(DataBuffer, BufferSize);
                }
                else{
                    //no data found???? weird?
                }
            }
            else{
                //First chunk was not IHDR, png file possibly corrupted??
            }

            return Result;
        }

        uint32 GetChannelCount(uint8 ColorType){
            //0-grayscale, 3-indexed
            if(ColorType == 0 || ColorType == 3){
                return 1;
            }
            //2-RGB
            else if(ColorType == 2){
                return 3;
            }
            //4-grayscale with alpha
            else if(ColorType == 4){
                return 2;
            }
            //6-RGBA
            else if(ColorType == 6){
                return 4;
            }
            return 0;
        }

        decompressed_data DecompressData(uint8* DataBuffer, uint32 BufferSize){
            decompressed_data Result = {};

            return Result;
        }

    }
}