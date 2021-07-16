#if defined(_WIN32)
#include "glimg_file.h"
#include "util.h"
#include <Windows.h>

namespace glimg
{
    namespace file
    {

        loaded_file::loaded_file(std::string Path) : ContentSize(0), Content(nullptr)
        {
            //get handle to specified file, spcifying that we want only get the handle to existing files, such that we wont create new files
            HANDLE FileHandle = CreateFileA(Path.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
            //Check if we got aq valid handle...
            if (FileHandle != INVALID_HANDLE_VALUE)
            {
                LARGE_INTEGER FileSize;
                //Get file size
                if (GetFileSizeEx(FileHandle, &FileSize))
                {
                    //Safely truncate 64bit size to 32bit, We will only support loading files under 4gb.
                    uint32 FileSize32 = util::TruncateUInt64ToUInt32(FileSize.QuadPart);
                    //We allocate memory required to load the file;
                    Content = VirtualAlloc(0, FileSize32, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                    //Check if our allocation succeeded
                    if (Content != nullptr)
                    {
                        DWORD BytesRead;
                        //We read the file, and store the contents in our allocated memory
                        if (ReadFile(FileHandle, Content, FileSize32, &BytesRead, 0) == TRUE && (BytesRead == FileSize32))
                        {
                            //We store the content size, so it can be returned to the user
                            ContentSize = FileSize32;
                        }
                        else
                        {
                            //Our read failed, either as a call, orwe did not read all the data from the file (ReadBytes != FileSize32)
                            FreeFile();
                        }
                    }
                }
                //Cleanup our handle
                CloseHandle(FileHandle);
            }
        }

        void loaded_file::FreeFile()
        {
            if (Content != nullptr)
            {
                VirtualFree(Content, 0, MEM_RELEASE);
                ContentSize = 0;
                Content = nullptr;
            }
        }

        loaded_file::~loaded_file()
        {
            FreeFile();
        }

    }
}

#endif //_WIN32