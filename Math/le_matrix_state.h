/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_matrix_state.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_MATERIAL_H
#define LE_MATERIAL_H

#include "../../glm/glm.hpp"
#include "../../glm/vec3.hpp"
#include "../../glm/vec4.hpp"
#include "../../glm/mat4x4.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/constants.hpp"
// Vulkan Includes
LE_NAMESPACE_BEGIN(Render)

class LEMatrixState :public LEObject, public Singleton<LEMatrixState> {
public:
	LEFloat currMatrix[16];
	LEFloat mProjMatrix[16];
	LEFloat mVMatrix[16];
	LEFloat mMVPMatrix[16];
	LEFloat vulkanClipMatrix[16];
	LEFloat mStack[10][16];
	LEFloat invertMvMatrix[16];
	LEFloat invertMMatrix[16];
	LEFloat helpArr[4];
	LEFloat helpResult[4];
	static LEInt stackTop;
	static LEFloat cx;
	static LEFloat cy;
	static LEFloat cz;
	static LEFloat tx;
	static LEFloat ty;
	static LEFloat tz;
	static void setInitStack();
	static void pushMatrix();
	static void popMatrix();
	static void translate(LEFloat x, LEFloat y, LEFloat z);
	static void rotate(LEFloat angle, LEFloat x, LEFloat y, LEFloat z);
	static void scale(LEFloat x, LEFloat y, LEFloat z);
	static void setCamera(
		LEFloat cx, LEFloat cy, LEFloat cz, // center
		LEFloat tx, LEFloat ty, LEFloat tz, // 
		LEFloat upx, LEFloat upy, LEFloat upz
	) {
		glm::fr
	}
	static void setProjectFrustum (
		LEFloat left,LEFloat right,LEFloat bottom,LEFloat top,
		LEFloat _near,LEFloat _far
	);
	static LEFloat left;
	static LEFloat right;
	static LEFloat bottom;
	static LEFloat top;
	static LEFloat nearSelf;
	static LEFloat farSelf;
	static LEFloat* getFinalMatrix();
	static LEFloat* getMMatrix();
	static void fromPtoPreP(LEFloat x, LEFloat y, LEFloat z, LEFloat* result);
};
LE_NAMESPACE_END

#endif // !LE_MATERIAL_H

