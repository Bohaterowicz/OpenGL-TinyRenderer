#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TextureCoords;
in vec3 v_Normal;
in vec3 v_FragPosition;

layout(std140) uniform ub_BaseMaterial {
  uniform vec3 u_Color;
  uniform float u_SpecularStrength;
  uniform int u_UseColorTexture;
};

uniform sampler2D ColorTextureSampler;
uniform vec3 LightPos;
uniform vec3 ViewPos;

vec3 AmbientIntensity = vec3(0.2, 0.2, 0.2);
vec3 LightColor = vec3(0.0, 1.0, 0.0);

void main() {
  vec3 FColor;
  if (u_UseColorTexture == 1) {
    FColor = texture(ColorTextureSampler, v_TextureCoords).rgb;
  } else {
    FColor = u_Color;
  }

  vec3 FragNormal = normalize(v_Normal);
  vec3 DirToLight = normalize(LightPos - v_FragPosition);
  float DiffuseLightIntensity = max(dot(FragNormal, DirToLight), 0.0);

  vec3 DirToView = normalize(ViewPos - v_FragPosition);
  vec3 ReflectDir = reflect(-DirToLight, FragNormal);

  float Spec = pow(max(dot(DirToView, ReflectDir), 0.0), 16);

  vec3 FLColor =
      clamp((FColor * DiffuseLightIntensity) + (FColor * AmbientIntensity) +
                (FColor * (LightColor * Spec * u_SpecularStrength)),
            0.0, 1.0);
  color = vec4(FLColor, 1.0);
}
