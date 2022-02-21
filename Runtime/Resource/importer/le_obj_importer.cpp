#include "le_obj_importer.h"

// Tiny Obj Includes
#define TINYOBJLOADER_IMPLEMENTATION 
#include <tinyobj/tiny_obj_loader.h>

LEObjImporter::LEObjImporter(const std::string& assetPath):LEAssetImporter(assetPath) {}

LEObjImporter::~LEObjImporter() {}

void LEObjImporter::loadModelData() {
	tinyobj::attrib_t                attrib;
	std::vector<tinyobj::shape_t>    shapes;
	std::vector<tinyobj::material_t> materials; 
	std::string                      errorStr;
	std::string                      warningStr;
	if (!tinyobj::LoadObj(&attrib,
		                  &shapes,
		                  &materials,
		                  &warningStr,
		                  &errorStr,
		                  this->m_path.c_str())) {
		// [Error]:throw error string
	}
	std::unordered_map<VertexData, uint32_t> uniqueVertices;

	for (auto& shape : shapes) {
		for (auto& index : shape.mesh.indices) {
			VertexData vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};
			if(!attrib.texcoords.empty()) {
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					// must be one minus
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}
			vertex.color = { 1.0f, 1.0f, 1.0f };
			if (uniqueVertices.find(vertex) == uniqueVertices.end()) {
				uniqueVertices[vertex] = static_cast<uint32_t>(this->m_vertices.size());
				this->m_vertices.push_back(vertex);
			}

			this->m_indices.push_back(uniqueVertices[vertex]);
		}
	}
}

LEMesh* LEObjImporter::registerAsset() const {
	const int vertexCount = this->m_vertices.size();
	if (vertexCount == 0) {
		return nullptr;
	}
	std::vector<glm::vec3> position(vertexCount);
	std::vector<glm::vec2> uv(vertexCount);
	for (int i = 0; i < vertexCount; i++) {
		position[i] = this->m_vertices[i].pos;
		uv[i] = this->m_vertices[i].texCoord;
	}
	VkDeviceSize bufferSize = sizeof(this->m_vertices[0]) * this->m_vertices.size();
	LEMesh* mesh = LEMesh::fromPositionUV(position, uv, this->m_indices);
	// [TODO]: register to asset manager
	return mesh;
}
