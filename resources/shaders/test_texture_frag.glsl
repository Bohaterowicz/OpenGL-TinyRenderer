#version 330 core

layout(location = 0) out vec4 color;


in vec4 v_SurfaceColor;
in vec2 v_TextureCoords;

uniform sampler2D Texture;


void main()
{
    color = vec4(texture(Texture, v_TextureCoords).r, 1.0, 1.0, 1.0);
}

