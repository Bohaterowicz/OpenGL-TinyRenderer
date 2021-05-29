#version 330 core

layout(location = 0) out vec4 color;


in vec4 v_SurfaceColor;
in vec3 v_CameraSpaceNormals;
in vec3 v_CameraSpacePosition;
in vec2 v_TextureCoords;


uniform bool u_UseSquaredAttenuationDistance;
uniform float u_LightAttenuation;
uniform vec3 u_CameraSpaceLight;
uniform sampler2D gaussianTexture;
uniform sampler2D shineTexture;

uniform ub_UnProjection{
    mat4 ClipToCameraMtx;
    ivec2 WindowSize;
};

vec4 LightIntensity = vec4(0.8, 0.8, 0.8, 1.0);
vec4 AmbientIntensity = vec4(0.2, 0.2, 0.2, 1.0);
const vec4 specularColor = vec4(0.5, 0.5, 0.5, 1.0);
const float shine = 0.1;

vec3 FragmentCameraSpacePosition() {
    vec4 NDCPos;
    NDCPos.xy = ((gl_FragCoord.xy / WindowSize.xy) * 2.0) - 1.0;
    NDCPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
    NDCPos.w = 1.0;

    vec4 ClipPos = NDCPos / gl_FragCoord.w;

    return vec3(ClipToCameraMtx * ClipPos);
}

float ComputeAttenuation(in vec3 FragmentCameraPos, out vec3 LightDirection) {
    vec3 LightVector = u_CameraSpaceLight - FragmentCameraPos;
    float LightDistanceSquared = dot(LightVector, LightVector);
    LightDirection = LightVector * inversesqrt(LightDistanceSquared);

    float DistanceFactor = u_UseSquaredAttenuationDistance ? LightDistanceSquared : sqrt(LightDistanceSquared);
    return (1.0 / (1.0 + DistanceFactor * u_LightAttenuation));
}

void main()
{
    float SpecShine = texture(shineTexture, v_TextureCoords).r;
    vec3 CameraSpaceFragment = FragmentCameraSpacePosition();
    vec3 LightDir = vec3(0.0);

    float Attenuation = ComputeAttenuation(CameraSpaceFragment, LightDir);
    vec4 AttenuatedLightIntensity = LightIntensity * Attenuation;

    float cosAngIncidence = dot(normalize(v_CameraSpaceNormals), LightDir);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    vec3 viewDirection = normalize(-v_CameraSpacePosition);
    vec3 HalfAngle = normalize(LightDir + viewDirection);
    vec2 texCoord;
    texCoord.s = dot(HalfAngle, v_CameraSpaceNormals);
    texCoord.t = SpecShine;
    float GaussianTerm = texture(gaussianTexture, texCoord).r;

    GaussianTerm = cosAngIncidence != 0.0 ? GaussianTerm : 0.0;

    color = (v_SurfaceColor * AttenuatedLightIntensity * cosAngIncidence) +
        (specularColor * AttenuatedLightIntensity * GaussianTerm) +
        (v_SurfaceColor * AmbientIntensity);

    
}

