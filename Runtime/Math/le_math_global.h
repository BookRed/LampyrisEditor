/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_math_global.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_MATH_GLOBAL_H
#define LE_MATH_GLOBAL_H

// GLM Includes
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_common.hpp>

#define LE_EPSILON 10e-4

#define LE_FLOAT_EQUAL(a,b) (glm::abs(a-b)<LE_EPSILON)

#define LE_FLOAT_ZERO(a) (glm::abs(a)<LE_EPSILON)
#endif // !LE_MATH_GLOBAL_H

