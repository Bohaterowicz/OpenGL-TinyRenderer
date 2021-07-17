#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tiny_renderer_c.h"
#include "tiny_renderer_input.h"

class base_camera
{
private:
	uint32 AssignedController;

	glm::vec3 ForwardVector;
	glm::vec3 Position;

	glm::mat4 *PerspectiveTransform;
	real32 FrustumScale;
	real32 AspectRatio;
	real32 NearPlane;
	real32 FarPlane;

public:
	base_camera(real32 FScale, real32 ARatio, real32 NPlane, real32 FPlane, uint32 ControllerIdx = 0);
	void UpdateAspectRatio(real32 ARatio);
	void UpdatePosition(glm::vec3 PositionChange);
	void UpdateForwardVector(glm::vec2 RotationChange);
	glm::mat4 GetCmameraTransformationMatrix() const;
	glm::mat4 &GetPerspectiveTransform();
	void ProcessInput(tiny_renderer_input &Input);

	inline glm::vec3 GetPosition() const { return Position; }

	//Static...
	static real32 FrustumScaleFromFOV(real32 FOVAngle);
};
