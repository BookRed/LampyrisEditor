#include "../../glm/mat4x4.hpp"
#include "../Common/le_serializable.h"

class LEMatrix4 :public glm::mat4,
	             public LEObject,
	             public Serializable {
public:
	BOOST_SERIALIZATION_SPLIT_MEMBER();
	
	LEMatrix4() {}

	LEMatrix4(const glm::vec4 &v1, 
		      const glm::vec4 &v2, 
		      const glm::vec4 &v3, 
		      const glm::vec4 &v4):glm::mat4(v1,v2,v3,v4) {}

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const {
		for (int i = 0; i < 4; i++) {
			ar << this->operator[](i).r;
			ar << this->operator[](i).g;
			ar << this->operator[](i).b;
			ar << this->operator[](i).a;
			
		}
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version) {
		for (int i = 0; i < 4; i++) {
			ar >> this->operator[](i).r;
			ar >> this->operator[](i).g;
			ar >> this->operator[](i).b;
			ar >> this->operator[](i).a;
		}
	}

	void inverse() {
		LEMatrix4& m = *this;
		float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		const glm::vec4 SignA = glm::vec4(1.0, -1.0, 1.0, -1.0);
		const glm::vec4 SignB = glm::vec4(-1.0, 1.0, -1.0, 1.0);

		glm::vec4 Fac0 = glm::vec4(Coef00, Coef00, Coef02, Coef03);
		glm::vec4 Fac1 = glm::vec4(Coef04, Coef04, Coef06, Coef07);
		glm::vec4 Fac2 = glm::vec4(Coef08, Coef08, Coef10, Coef11);
		glm::vec4 Fac3 = glm::vec4(Coef12, Coef12, Coef14, Coef15);
		glm::vec4 Fac4 = glm::vec4(Coef16, Coef16, Coef18, Coef19);
		glm::vec4 Fac5 = glm::vec4(Coef20, Coef20, Coef22, Coef23);

		glm::vec4 Vec0 = glm::vec4(m[1][0], m[0][0], m[0][0], m[0][0]);
		glm::vec4 Vec1 = glm::vec4(m[1][1], m[0][1], m[0][1], m[0][1]);
		glm::vec4 Vec2 = glm::vec4(m[1][2], m[0][2], m[0][2], m[0][2]);
		glm::vec4 Vec3 = glm::vec4(m[1][3], m[0][3], m[0][3], m[0][3]);

		glm::vec4 Inv0 = SignA * (Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		glm::vec4 Inv1 = SignB * (Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		glm::vec4 Inv2 = SignA * (Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		glm::vec4 Inv3 = SignB * (Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		LEMatrix4 Inverse = LEMatrix4(Inv0, Inv1, Inv2, Inv3);

		glm::vec4 Row0 = glm::vec4(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		LEFloat Determinant = dot(m[0], Row0);

		Inverse /= Determinant;
	}

	LEMatrix4 inversed() const {
		LEMatrix4 result(*this);
		result.inverse();
		return result;
	}
};