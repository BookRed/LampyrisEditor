#include <glm/vec4.hpp>
#include "../Common/le_serializable.h"

LE_NAMESPACE_BEGIN(Math)
class LEVector4 :public glm::vec4,
				 public Serializable {
public:
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const {
		ar << glm::vec4::x;
		ar << glm::vec4::y;
		ar << glm::vec4::z;
		ar << glm::vec4::w;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version) {
		ar >> glm::vec4::x;
		ar >> glm::vec4::y;
		ar >> glm::vec4::z;
		ar >> glm::vec4::w;
	}
};
LE_NAMESPACE_END;
