#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 vertColor;

out vec2 v_TexCoord;
smooth out vec4 v_VertColors;

uniform float u_LoopDuration;
uniform float u_Time;

void main()
{
	float Scale = 3.14159f * 2.0f / u_LoopDuration;

	float ElapsedTime = u_Time / 1.0f;

	float CurrTimeThroughLoop = mod(ElapsedTime, u_LoopDuration);

	float OffsetX = cos(CurrTimeThroughLoop * Scale) * 0.5f;
	float OffsetY = sin(CurrTimeThroughLoop * Scale) * 0.5f;
	vec4 AdjustedOffset = vec4(OffsetX, OffsetY, 0.0, 0.0);
	gl_Position = position + AdjustedOffset;
	v_TexCoord = texCoord;
	v_VertColors = vertColor;
}