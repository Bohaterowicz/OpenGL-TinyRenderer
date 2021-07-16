#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_SurfaceColor;
in vec2 v_TextureCoords;

void main() { color = vec4(0.0, 0.0, 0.0, 1.0); }
