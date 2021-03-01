#version 330 core

layout(location = 0) out vec4 color;


in vec4 v_SurfaceColor;
in vec3 v_CameraSpaceNormals;


uniform bool u_UseSquaredAttenuationDistance;
uniform float u_LightAttenuation;
uniform vec3 u_CameraSpaceLight;

uniform ub_UnProjection{
    mat4 ClipToCameraMtx;
    ivec2 WindowSize;
};

vec4 LightIntensity = vec4(0.8, 0.8, 0.8, 1.0);
vec4 AmbientIntensity = vec4(0.1, 0.1, 0.1, 1.0);

vec3 FragmentCameraSpacePosition() {
    vec4 NDCPos;
    NDCPos.xy = ((gl_FragCoord.xy / WindowSize.xy) * 2.0) - 1.0;
    NDCPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
    NDCPos.w = 1.0;

    vec4 ClipPos = NDCPos / gl_FragCoord.w;

    return vec3(ClipToCameraMtx * ClipPos);
}

vec4 ApplyLightIntensity(in vec3 FragmentCameraPos, out vec3 LightDirection) {
    vec3 LightVector = u_CameraSpaceLight - FragmentCameraPos;
    float LightDistanceSquared = dot(LightVector, LightVector);
    LightDirection = LightVector * inversesqrt(LightDistanceSquared);

    float DistanceFactor = u_UseSquaredAttenuationDistance ? LightDistanceSquared : sqrt(LightDistanceSquared);
    return LightIntensity * (1.0 / (1.0 + DistanceFactor * u_LightAttenuation));
}

void main()
{
    vec3 CameraSpaceFragment = FragmentCameraSpacePosition();
    vec3 LightDir = vec3(0.0);
    vec4 AttenuatedLightIntensity = ApplyLightIntensity(CameraSpaceFragment, LightDir);

    float cosAngIncidence = dot(normalize(v_CameraSpaceNormals), LightDir);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	color = (v_SurfaceColor * AttenuatedLightIntensity * cosAngIncidence) +
        (v_SurfaceColor * AmbientIntensity);
}
