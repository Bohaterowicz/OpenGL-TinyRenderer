#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_SurfaceColor;
in vec2 v_TextureCoords;

uniform sampler2D checkerTexture;
uniform vec3 u_Color;
uniform int u_UseTexture;

void main() {
  vec3 FColor;
  if (u_UseTexture == 1) {
    FColor = texture(checkerTexture, v_TextureCoords).rgb;
  } else {
    FColor = u_Color;
  }
  color = vec4(FColor, 1.0);
}
