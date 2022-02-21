#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_obj_importer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_asset_importer.h"
#include "../le_mesh.h"

// STL Includes
#include <vector>
#include <unordered_map>
#include <algorithm>

// GLM Includes
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp> // enable std::hash<glm::xxx>

struct VertexData {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	bool operator==(const VertexData& other) const {
		return pos      == other.pos     && 
			   color    == other.color   && 
			   texCoord == other.texCoord;
	}
};
namespace std {
	template<> struct hash<VertexData> {
		size_t operator()(VertexData const& vertex) const {
			return ((std::hash<glm::vec3>()(vertex.pos) ^ 
				    (std::hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ 
				    (std::hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}
class LEObjImporter:public LEAssetImporter<LEMesh>{
private:
	std::vector<VertexData>   m_vertices;
	std::vector<int>          m_indices;
public:
	      LEObjImporter(const std::string& assetPath);

	      ~LEObjImporter();

	void loadModelData();

	LEMesh* registerAsset() const;
};

