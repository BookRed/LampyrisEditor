#include "../../glm/qua.hpp"
#include "../Common/le_serializable.h"

class LEQuaternion :public glm::qua,
				    public Serializable {
public:
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const {
		ar << glm::qua::x;
		ar << glm::qua::y;
		ar << glm::qua::z;
		ar << glm::qua::w;
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version) {
		ar >> glm::qua::x;
		ar >> glm::qua::y;
		ar >> glm::qua::z;
		ar >> glm::qua::w;
	}
};