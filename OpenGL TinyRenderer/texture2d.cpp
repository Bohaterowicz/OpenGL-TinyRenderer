#include "texture2d.h"
#include"stb_image.h"
#include "opengl_util.h"

texture2d::texture2d(std::string Path) : GLID(0), TexturePath(Path), PixelBuffer(nullptr), Width(0), Height(0), BytesPerPixel(0)
{
	stbi_set_flip_vertically_on_load(1);

	PixelBuffer = stbi_load(Path.c_str(), &Width, &Height, &BytesPerPixel, 4);

	GLCall(glGenTextures(1, &GLID));
	GLCall(glBindTexture(GL_TEXTURE_2D, GLID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, PixelBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (PixelBuffer) {
		stbi_image_free(PixelBuffer);
	}
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
