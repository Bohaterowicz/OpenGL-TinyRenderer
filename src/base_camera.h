#pragma once
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tiny_renderer_c.h"

class base_camera
{
private:
	glm::vec3 ForwardVector;
	glm::vec3 Position;

	glm::mat4* PerspectiveTransform;
	real32 FrustumScale;
	real32 AspectRatio;
	real32 NearPlane;
	real32 FarPlane;
public:
	base_camera(real32 FScale, real32 ARatio, real32 NPlane, real32 FPlane);
	void UpdateAspectRatio(real32 ARatio);
	void UpdatePosition(glm::vec3 PositionChange);
	void UpdateForwardVector(glm::vec2 RotationChange);
	glm::mat4 GetCmameraTransformationMatrix();
	glm::mat4& GetPerspectiveTransform();


public:
	//Static...
	static real32 FrustumScaleFromFOV(real32 FOVAngle);
};

