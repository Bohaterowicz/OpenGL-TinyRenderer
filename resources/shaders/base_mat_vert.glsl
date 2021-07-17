#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 tex_coords;

out vec2 v_TextureCoords;
out vec3 v_Normal;
out vec3 v_FragPosition;

layout(std140) uniform ub_MVP {
  uniform mat4 u_ModelTransformMtx;
  uniform mat4 u_WorldToCameraMtx;
  uniform mat4 u_CameraToClipMtx;
};

void main() {

  vec4 VertPos = vec4(position, 1.0);
  vec4 WorldSpace = u_ModelTransformMtx * VertPos;
  vec4 ViewSpace = u_WorldToCameraMtx * WorldSpace;
  vec4 ClipSpace = u_CameraToClipMtx * ViewSpace;

  gl_Position = ClipSpace;

  // We send the texture coordinates to fragment sahder
  v_TextureCoords = tex_coords;
  // We send normals to frag shader, notice the normals are in world space, if
  // we were to use non uniform scaling in the model
  // transform matrix, we would need to adjust our normals with the
  // transpose-invers of the transformation matrix...
  v_Normal = normals;
  // We send the vertex position in world space, to our fragment wich will
  // become fragment positions in fragment shaders that are in world space. We
  // do this so that we can perform lighting calculations in world space.
  v_FragPosition = vec3(WorldSpace);
}