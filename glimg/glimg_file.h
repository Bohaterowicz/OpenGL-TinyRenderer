#pragma once
#include "glimg_c.h"

#include<string>

namespace glimg{
    namespace file{
        /*
        Loads file from given path.
        If contents pointer is nullptr, this means that read failed. 
        */
        class loaded_file{
            private:
            size ContentSize;
            void *Content;

            void FreeFile();

            public:
            loaded_file(std::string Path);
            ~loaded_file();

            inline size GetSize(){return ContentSize;};
            inline void* GetData(){return Content;};
        };
    }
}