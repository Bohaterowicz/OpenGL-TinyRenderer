#pragma once
#include "tiny_renderer_c.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class quaternion
{
private:
    glm::quat Quaternion;

public:
    quaternion() {}
    quaternion(real32 X, real32 Y, real32 Z, real32 W) : Quaternion(W, X, Y, Z) {}
    quaternion(glm::vec3 EulerAngle) : Quaternion(glm::quat(EulerAngle)) {}
    inline void SetFromEulerAngle(glm::vec3 EulerAngle) { Quaternion = glm::quat(EulerAngle); }
    inline glm::mat4 ToRotationMatrix() const { return glm::toMat4(Quaternion); }
};