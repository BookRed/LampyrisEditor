#include "le_math.h"
#include "le_vector4.h"
#include "../Common/le_serializable.h"

LE_NAMESPACE_BEGIN(Math)
class LEColor:public Serializable {
private:
	LEVector4 m_rgba;
public:
	LEColor() {
		this->m_rgba.r = 0.0f;
		this->m_rgba.g = 0.0f;
		this->m_rgba.b = 0.0f;
		this->m_rgba.a = 1.0f;
	}
	LEColor(LEFloat r,LEFloat g,LEFloat b,LEFloat a = 1.0f) {
		this->m_rgba.r =  clamp(0.0f,1.0f,r);
		this->m_rgba.g =  clamp(0.0f,1.0f,g);
		this->m_rgba.b =  clamp(0.0f,1.0f,b);
		this->m_rgba.a =  clamp(0.0f,1.0f,a);
	}
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const {
		ar << m_rgba;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version) {
		ar >> m_rgba;
	}

	LE_FORCEINLINE LEFloat red()   const { return this->m_rgba.r; }
	LE_FORCEINLINE LEFloat green() const { return this->m_rgba.g; }
	LE_FORCEINLINE LEFloat blue()  const { return this->m_rgba.b; }
	LE_FORCEINLINE LEFloat alpha() const { return this->m_rgba.a; }

	LE_FORCEINLINE LEFloat setRed(LEFloat value)   { this->m_rgba.r = clamp(0.0f, 1.0f, value); }
	LE_FORCEINLINE LEFloat setGreen(LEFloat value) { this->m_rgba.g = clamp(0.0f, 1.0f, value); }
	LE_FORCEINLINE LEFloat setBlue(LEFloat value)  { this->m_rgba.b = clamp(0.0f, 1.0f, value); }
	LE_FORCEINLINE LEFloat setAlpha(LEFloat value) { this->m_rgba.a = clamp(0.0f, 1.0f, value); }
};

LE_NAMESPACE_END;
