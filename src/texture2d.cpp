#include "texture2d.h"
#include"stb_image.h"
#include "opengl_util.h"

texture2d::texture2d(uint8* Data, int32 _Width, int32 _Height, int32 BPP) : GLID(0), PixelBuffer(Data), Width(_Width), Height(_Height), BytesPerPixel(BPP)
{
	//stbi_set_flip_vertically_on_load(1);

	

	GLCall(glGenTextures(1, &GLID));
	GLCall(glBindTexture(GL_TEXTURE_2D, GLID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, PixelBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	/*
	if (PixelBuffer) {
		stbi_image_free(PixelBuffer);
	}
	*/
}

texture2d::texture2d(std::string Path, bool32 Mipmaps, uint32 MipmapCount, uint32 ChannelCount){
	GLCall(glGenTextures(1, &GLID));
	GLCall(glBindTexture(GL_TEXTURE_2D, GLID));

	stbi_set_flip_vertically_on_load(1);
	if (Mipmaps){
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, MipmapCount - 1));

		std::string MipmapPath = Path + "_";
		for(uint32 i = 0; i < MipmapCount; ++i){
			std::string tmpMipmapPath = MipmapPath;
			tmpMipmapPath += std::to_string(i);
			tmpMipmapPath += ".png";
			PixelBuffer = stbi_load(tmpMipmapPath.c_str(), &Width, &Height, &BytesPerPixel, ChannelCount);
			GLCall(glTexImage2D(GL_TEXTURE_2D, i, GL_R8, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, PixelBuffer));
		}
	}
	else{
		PixelBuffer = stbi_load(Path.c_str(), &Width, &Height, &BytesPerPixel, ChannelCount);
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, PixelBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

texture2d::~texture2d()
{
	GLCall(glDeleteTextures(1, &GLID));
}

void texture2d::Bind(uint32 BindSlot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + BindSlot));
	GLCall(glBindTexture(GL_TEXTURE_2D, GLID));
}

void texture2d::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
