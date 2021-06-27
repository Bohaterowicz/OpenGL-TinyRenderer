#include "base_camera.h"

base_camera::base_camera(real32 FScale, real32 ARatio, real32 NPlane, real32 FPlane, uint32 ControllerIdx) : AssignedController(ControllerIdx), FrustumScale(FScale), AspectRatio(ARatio), NearPlane(NPlane), FarPlane(FPlane)
{
	Position = glm::vec3(0.0F, 300.0F, 0.0F);
	ForwardVector = glm::vec3(-1.0F, 0.0F, 0.0F);
	PerspectiveTransform = new glm::mat4(0.0F);
	(*PerspectiveTransform)[0].x = FrustumScale / AspectRatio;
	(*PerspectiveTransform)[1].y = FrustumScale;
	(*PerspectiveTransform)[2].z = (FarPlane + NearPlane) / (NearPlane - FarPlane);
	(*PerspectiveTransform)[3].z = (2 * FarPlane * NearPlane) / (NearPlane - FarPlane);
	(*PerspectiveTransform)[2].w = -1.0F;
}

void base_camera::UpdateAspectRatio(real32 ARatio)
{
	AspectRatio = ARatio;
	(*PerspectiveTransform)[0].x = FrustumScale / AspectRatio;
}

void base_camera::UpdatePosition(glm::vec3 PositionChange)
{
	Position += PositionChange.x * ForwardVector;
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 RightVector = glm::normalize(glm::cross(ForwardVector, UpVector));
	Position += PositionChange.y * RightVector;
	glm::vec3 CameraUpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
	Position += PositionChange.z * CameraUpVector;
}

void base_camera::UpdateForwardVector(glm::vec2 RotationChange)
{
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 RightVector = glm::normalize(glm::cross(ForwardVector, UpVector));
	glm::vec3 CameraUpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
	glm::mat4 rotMtx = glm::rotate(glm::mat4(1.0f), glm::radians(RotationChange.y * 0.05f), RightVector) * glm::rotate(glm::mat4(1.0f), glm::radians(RotationChange.x * 0.05f), CameraUpVector);
	ForwardVector = glm::normalize(glm::vec3(glm::vec4(ForwardVector, 1.0f) * rotMtx));
}

glm::mat4 base_camera::GetCmameraTransformationMatrix()
{
	glm::mat4 CameraTransformationMatrix = glm::mat4(1.0f);
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 RightVector = glm::normalize(glm::cross(ForwardVector, UpVector));
	glm::vec3 CameraUpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
	CameraTransformationMatrix[3] = glm::vec4(-Position, 1.0f);
	glm::mat4 CameraRotationMtx = glm::mat4(1.0f);
	CameraRotationMtx[0] = glm::vec4(RightVector, 0.0f);
	CameraRotationMtx[1] = glm::vec4(CameraUpVector, 0.0f);
	CameraRotationMtx[2] = glm::vec4(-ForwardVector, 0.0f);
	CameraRotationMtx = glm::transpose(CameraRotationMtx);
	CameraTransformationMatrix = CameraRotationMtx * CameraTransformationMatrix;
	return CameraTransformationMatrix;
}

glm::mat4 &base_camera::GetPerspectiveTransform()
{
	return *PerspectiveTransform;
}

void base_camera::ProcessInput(tiny_renderer_input &Input)
{
	glm::vec3 UpdatePos = {};
	if (Input.Controller[0].MoveForward.EndedDown == TRUE)
	{
		UpdatePos.x += 2.0F;
	}
	if (Input.Controller[0].MoveBack.EndedDown == TRUE)
	{
		UpdatePos.x -= 2.0F;
	}
	if (Input.Controller[0].MoveRight.EndedDown == TRUE)
	{
		UpdatePos.y += 2.0F;
	}
	if (Input.Controller[0].MoveLeft.EndedDown == TRUE)
	{
		UpdatePos.y -= 2.0F;
	}
	if (Input.Controller[0].MoveUp.EndedDown == TRUE)
	{
		UpdatePos.z += 2.0F;
	}
	if (Input.Controller[0].MoveDown.EndedDown == TRUE)
	{
		UpdatePos.z -= 2.0F;
	}

	UpdatePosition(UpdatePos);

	glm::vec2 CameraRotChange = glm::vec2(Input.Mouse.dX, Input.Mouse.dY);
	UpdateForwardVector(CameraRotChange);
}

real32 base_camera::FrustumScaleFromFOV(real32 FOVAngle)
{
	constexpr float degToRad = 3.14159f * 2.0f / 360.0f;
	float fFovRad = FOVAngle * degToRad;
	return 1.0f / tan(fFovRad / 2.0f);
	;
}
