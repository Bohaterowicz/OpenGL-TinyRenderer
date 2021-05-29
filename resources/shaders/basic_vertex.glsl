#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 tex_coords;


out vec4 v_SurfaceColor;
out vec3 v_CameraSpaceNormals;
out vec3 v_CameraSpacePosition;
out vec2 v_TextureCoords;

layout(std140) uniform ub_GlobalMatrices
{
	uniform mat4 u_WorldToCameraMtx;
	uniform mat4 u_CameraToClipMtx;
};



vec4 SurfaceColor = vec4(0.3, 0.3, 0.5, 1.0);
vec4 SurfaceColor2 = vec4(0.3, 0.3, 0.3, 1.0);

void main()
{
	vec4 VertPos = vec4(position, 1.0);
	vec4 ModelSpace = VertPos;
	vec4 ViewSpace = u_WorldToCameraMtx * ModelSpace;

	v_SurfaceColor = SurfaceColor2;
	v_CameraSpaceNormals = mat3(u_WorldToCameraMtx) * normals;

	vec4 ClipSpace = u_CameraToClipMtx * ViewSpace;
	gl_Position = ClipSpace;
	v_CameraSpacePosition = vec3(u_WorldToCameraMtx * vec4(position, 1.0));

	v_TextureCoords = tex_coords;
}