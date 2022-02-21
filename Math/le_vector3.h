#include <glm/vec3.hpp>
#include "../Common/le_serializable.h"

LE_NAMESPACE_BEGIN(Math)
class LEVector3 :public glm::vec3,
				 public Serializable {
public:
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const {
		ar << glm::vec3::x;
		ar << glm::vec3::y;
		ar << glm::vec3::z;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version) {
		ar >> glm::vec3::x;
		ar >> glm::vec3::y;
		ar >> glm::vec3::z;
	}
};

LE_NAMESPACE_END;
