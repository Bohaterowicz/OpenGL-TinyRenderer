#version 330 core

layout(location = 0) out vec4 color;


in vec4 v_SurfaceColor;
in vec2 v_TextureCoords;

uniform sampler2D checkerTexture;


void main()
{
    float val =  texture(checkerTexture, v_TextureCoords).r;
    color = vec4(val, val, val, 1.0);
}

