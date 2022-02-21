#include <glm/vec2.hpp>
#include "../Common/le_serializable.h"

LE_NAMESPACE_BEGIN(Math)
class LEVector2 :public glm::vec2,
	             public Serializable {
public:
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const {
		ar << glm::vec2::x;
		ar << glm::vec2::y;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version) {
		ar >> glm::vec2::x;
		ar >> glm::vec2::y;
	}
};
LE_NAMESPACE_END;
