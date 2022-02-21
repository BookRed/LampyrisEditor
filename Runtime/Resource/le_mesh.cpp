/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Resource
 * \File:    le_mesh.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// Vulkan Includes
#include <vulkan/vulkan.h>
// LE Includes
#include "le_mesh.h"
#include "../Renderer/le_vk_buffer.h"

LEMesh* LEMesh::ms_pMeshCube = nullptr;
LEMesh* LEMesh::ms_pMeshPlane = nullptr;
LEMesh* LEMesh::ms_pMeshSphere = nullptr;

LEVertexBuffer::LEVertexBuffer(std::vector<VertexFormat> formats, int vertexCount) {
	for (int i = 0; i < VF_MAX; i++) {
		this->m_formatOffset[i] = 0;
	}
	int byteCountPerVertex = 0;
	for (int i = 0; i < formats.size(); i++) {
		this->m_formatOffset[formats[i]] = byteCountPerVertex;
		byteCountPerVertex += s_byteCountMap[formats[i]];
	}
	if(!this->hasFormat(VF_NORMAL)) {
		this->m_formatOffset[VF_NORMAL] = byteCountPerVertex;
		byteCountPerVertex += s_byteCountMap[VF_NORMAL];
	}
	this->m_buffer = new LEVkBuffer(byteCountPerVertex * vertexCount,
	                                VK_BUFFER_USAGE_TRANSFER_DST_BIT | 
	                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	this->m_vertexCount = vertexCount;
	this->m_byteCountPerVertex = byteCountPerVertex;
}

LEVertexBuffer::~LEVertexBuffer() {
	LE_SAFE_RELEASE(this->m_buffer);
}

bool LEVertexBuffer::hasFormat(VertexFormat format) const {
	return this->m_formatOffset[format] != -1;
}

int LEVertexBuffer::getVertexCount() const {
	return this->m_vertexCount;
}

void LEVertexBuffer::setVertexValueAt(unsigned char* data, int byteCountPerVertex, int index,
	VertexFormat format) const {
	this->m_buffer->map();
	this->m_buffer->writeByte(data, byteCountPerVertex, this->m_byteCountPerVertex * index + this->m_formatOffset[format]);
	this->m_buffer->unmap();
}

void LEVertexBuffer::setVertexValue(unsigned char* data, int byteCountPerVertex, VertexFormat format) const {
	this->m_buffer->map();
	this->m_buffer->writeByte(data,
	                          s_byteCountMap[format] * this->m_vertexCount, 
	                          this->m_vertexCount * this->m_formatOffset[format]);
	this->m_buffer->unmap();
}

void LEVertexBuffer::setPositionAt(const glm::vec3& position, int index) const {
	this->setVertexValueAt((unsigned char*)(&position), sizeof(glm::vec3), index, VF_POSITION);
}

void LEVertexBuffer::setPosition(const std::vector<glm::vec3>& position) const {
	this->setVertexValue((unsigned char*)(position.data()), sizeof(glm::vec3), VF_POSITION);
}

void LEVertexBuffer::setColorAt(const glm::vec4& color, int index) const {
	this->setVertexValueAt((unsigned char*)(&color), sizeof(glm::vec4), index, VF_COLOR);
}

void LEVertexBuffer::setColor(const std::vector<glm::vec4>& color) const {
	this->setVertexValue((unsigned char*)(color.data()), sizeof(glm::vec4), VF_COLOR);
}

void LEVertexBuffer::setTexCoorAt(const glm::vec2& texCoor, int index) const {
	this->setVertexValueAt((unsigned char*)(&texCoor), sizeof(glm::vec2), index, VF_TEXCOOR);
}

void LEVertexBuffer::setTexCoor(const std::vector<glm::vec2>& uv) const {
	this->setVertexValue((unsigned char*)(uv.data()), sizeof(glm::vec2), VF_TEXCOOR);
}

void LEVertexBuffer::setNormal(const std::vector<glm::vec3>& normals) const {
	this->setVertexValue((unsigned char*)(normals.data()), sizeof(glm::vec3), VF_NORMAL);
}

LEVkBuffer* LEVertexBuffer::get() {
	return this->m_buffer;
}

uint32_t LEVertexBuffer::getFormatOffset(VertexFormat format) const {
	return this->m_formatOffset[format];
}

LEIndexBuffer::LEIndexBuffer(const std::vector<int>& indices) {
	this->m_buffer = new LEVkBuffer(indices.size() * sizeof(int), 
	                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
	                                VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	this->m_indexCount = indices.size();
}

int LEIndexBuffer::getIndexCount() const {
	return this->m_indexCount;
}

void LEIndexBuffer::setIndexData(const std::vector<int>& indices) {
	this->m_buffer->map();
	this->m_buffer->writeByte((unsigned char*)(indices.data()), sizeof(int) * this->m_indexCount, 0);
	this->m_buffer->unmap();
}

LEVkBuffer* LEIndexBuffer::get() {
	return this->m_buffer;
}

LEMesh::LEMesh() {}

void LEMesh::calculateAABB(const std::vector<glm::vec3>& position) {
	if(!this->m_pAABB) {
		this->m_pAABB = new LEAABB(position);
	}
}

LEResource::ResourceType LEMesh::getType() const {
	return RT_MESH;
}

LEAABB* LEMesh::getAABB() {
	return this->m_pAABB;
}

void LEMesh::onLoaded() {
		
}

void LEMesh::onUnloaded() {
	LE_SAFE_RELEASE(this->m_vertexBuffer);
	LE_SAFE_RELEASE(this->m_indexBuffer);
}

LEMesh::~LEMesh() {
	LE_SAFE_RELEASE(this->m_vertexBuffer);
	LE_SAFE_RELEASE(this->m_indexBuffer);
}

LEVertexBuffer* LEMesh::getVertexBuffer() {
	return this->m_vertexBuffer;
}

LEIndexBuffer* LEMesh::getIndexBuffer() {
	return this->m_indexBuffer;
}

LEMesh* LEMesh::fromPosition(const std::vector<glm::vec3>& position, const std::vector<int>& indices) {
	int vertexCount = position.size();
	if (vertexCount == 0) {
		return nullptr;
	}
	LEMesh* pMesh = new LEMesh;
	pMesh->m_vertexBuffer = new LEVertexBuffer({ LEVertexBuffer::VF_POSITION }, vertexCount);
	pMesh->m_vertexBuffer->setPosition(position);
	pMesh->m_indexBuffer  = new LEIndexBuffer(indices);
	pMesh->m_indexBuffer->setIndexData(indices);
	pMesh->m_vertexBuffer->setNormal(calculateNormal(position, indices));
	pMesh->calculateAABB(position);
	return pMesh;
}

LEMesh* LEMesh::fromPosition(const std::vector<glm::vec3>& position) {
	int vertexCount = position.size();
	if (vertexCount == 0) {
		return nullptr;
	}
	std::vector<glm::vec3> uniquePositions;
	std::vector<int> indices;
	indices.resize(vertexCount);
	std::unordered_map<glm::vec3, int> uniqueVertices;
	// remove repeated vertices
	for (int i = 0; i < vertexCount; i++) {
		auto& pos = position[i];
		int   index = -1;
		if (!uniqueVertices.contains(pos)) {
			index = uniqueVertices[pos] = uniqueVertices.size();
			uniquePositions.push_back(pos);
		}
		else {
			index = uniqueVertices[pos];
		}
		indices[i] = index;
	}
	LEMesh* pMesh = new LEMesh;
	pMesh->m_vertexBuffer = new LEVertexBuffer({ LEVertexBuffer::VF_POSITION }, vertexCount);
	pMesh->m_vertexBuffer->setPosition(uniquePositions);
	pMesh->m_indexBuffer  = new LEIndexBuffer(indices);
	pMesh->m_indexBuffer->setIndexData(indices);
	pMesh->m_vertexBuffer->setNormal(calculateNormal(position, indices));
	pMesh->calculateAABB(position);
	return pMesh;
}

LEMesh* LEMesh::fromPositionColor(const std::vector<glm::vec3>& position, const std::vector<glm::vec4> color, const std::vector<int>& indices) {
	const unsigned int vertexCount = position.size();
	if (vertexCount == 0) {
		return nullptr;
	}
	LEMesh* pMesh = new LEMesh;
	pMesh->m_vertexBuffer = new LEVertexBuffer({ LEVertexBuffer::VF_POSITION,LEVertexBuffer::VF_COLOR }, vertexCount);
	pMesh->m_vertexBuffer->setPosition(position);
	pMesh->m_vertexBuffer->setColor(color);
	pMesh->m_indexBuffer  = new LEIndexBuffer(indices);
	pMesh->m_indexBuffer->setIndexData(indices);
	pMesh->calculateAABB(position);
	return pMesh;
}

LEMesh* LEMesh::fromPositionUV(const std::vector<glm::vec3>& position, const std::vector<glm::vec2> uv, const std::vector<int>& indices) {
	unsigned int vertexCount = position.size();
	if (vertexCount == 0) {
		return nullptr;
	}
	LEMesh* pMesh = new LEMesh;
	pMesh->m_vertexBuffer = new LEVertexBuffer({ LEVertexBuffer::VF_POSITION,LEVertexBuffer::VF_TEXCOOR,LEVertexBuffer::VF_NORMAL}, vertexCount);
	pMesh->m_vertexBuffer->setPosition(position);
	pMesh->m_vertexBuffer->setTexCoor(uv);
	pMesh->m_indexBuffer  = new LEIndexBuffer(indices);
	pMesh->m_indexBuffer->setIndexData(indices);
	pMesh->m_vertexBuffer->setNormal(calculateNormal(position, indices));
	pMesh->calculateAABB(position);
	return pMesh;
}

LEMesh* LEMesh::fromPositionNormalUV(const std::vector<glm::vec3>& position, const std::vector<glm::vec3> normal,
								     const std::vector<glm::vec2> uv, const std::vector<int>& indices) {
	uint32_t vertexCount = position.size();
	if (vertexCount == 0) {
		return nullptr;
	}
	LEMesh* pMesh = new LEMesh;
	pMesh->m_vertexBuffer = new LEVertexBuffer(
		{LEVertexBuffer::VF_POSITION, LEVertexBuffer::VF_NORMAL, LEVertexBuffer::VF_TEXCOOR},
		vertexCount);
	pMesh->m_vertexBuffer->setPosition(position);
	pMesh->m_vertexBuffer->setTexCoor(uv);
	pMesh->m_vertexBuffer->setNormal(normal);
	if(!indices.empty()) {
		pMesh->m_indexBuffer = new LEIndexBuffer(indices);
		pMesh->m_indexBuffer->setIndexData(indices);
	}
	pMesh->calculateAABB(position);
	return pMesh;
}

std::vector<glm::vec3> LEMesh::calculateNormal(const std::vector<glm::vec3>& position, const std::vector<int>& indices) {
	const int vertexCount = position.size();
	const int indexCount  = indices.size();
	std::vector<glm::vec3> normals(vertexCount);
	for (int i = 0; i < indexCount / 3; ++i) {
		glm::vec3 v1 = position[indices[3 * i + 1]] - position[indices[3 * i + 0]];
		glm::vec3 v2 = position[indices[3 * i + 2]] - position[indices[3 * i + 0]];
		glm::vec3 v3 = glm::normalize(glm::cross(v1, v2));
		normals[indices[3 * i + 0]] += v3;
		normals[indices[3 * i + 1]] += v3;
		normals[indices[3 * i + 2]] += v3;
	}
	for (int i = 0; i < vertexCount; ++i) {
		normals[i] = glm::normalize(normals[i]);
	}
	return normals;
}

LEMesh* LEMesh::getCubeMesh() {
	createBuildInResources();
	return ms_pMeshCube;
}

LEMesh* LEMesh::getPlaneMesh() {
	createBuildInResources();
	return ms_pMeshPlane;
}

LEMesh* LEMesh::getSphereMesh() {
	createBuildInResources();
	return ms_pMeshSphere;
}

void LEMesh::createBuildInResources() {
	// Cube
	/*      
             4    FRONT     7
             ______________ 
            / |           /|
           /  |  UP    3 / |
        0 /___|_________/  |
   LEFT   |  5|_________|__|6  RIGHT
          |  /          |  /
          | /   BOTTOM  | /
          |/____________|/        
        1              2   
                BACK   
        */
	if(!ms_pMeshCube) {
		std::vector<int> indices(36);
		for (uint32_t i = 0; i < 36; i++) {
			indices[i] = i;
		}
		auto vertices = std::vector<glm::vec3>{
			{-1, 1, -1}, { -1,-1,-1 }, { 1,-1,-1 }, { 1,-1,-1 }, { 1,1,-1 }, { -1,1,-1 },
			{ -1,1,1 }, { -1,1,-1 }, { 1,1,-1 }, { 1,1,-1 }, { 1,1,1 }, { -1,1,1 },
			{ 1,1,-1 }, { 1,-1,-1 }, { 1,-1,1 }, { 1,-1,1 }, { 1,1,1 }, { 1,1,-1 },
			{ -1,1,1 }, { -1,-1,1 }, { -1,-1,-1 }, { -1,-1,-1 }, { -1,1,-1 }, { -1,1,1 },
			{ -1,-1,-1 }, { -1,-1,1 }, { 1,-1,1 }, { 1,-1,1 }, { 1,-1,-1 }, { -1,-1,-1 },
			{ 1,1,1 }, { 1,-1,1 }, { -1,-1,1 }, { -1,-1,1 }, { -1,1,1 }, { 1,1,1 }
		};
		auto normal = vertices;
		auto uv = std::vector<glm::vec2>{
			{ 0, 0 }, { 0,1 }, { 1,1 }, { 1,1 }, { 1,0 }, { 0,0 },
			{ 0, 0 }, { 0,1 }, { 1,1 }, { 1,1 }, { 1,0 }, { 0,0 },
			{ 0, 0 }, { 0,1 }, { 1,1 }, { 1,1 }, { 1,0 }, { 0,0 },
			{ 0, 0 }, { 0,1 }, { 1,1 }, { 1,1 }, { 1,0 }, { 0,0 },
			{ 0, 0 }, { 0,1 }, { 1,1 }, { 1,1 }, { 1,0 }, { 0,0 },
			{ 0, 0 }, { 0,1 }, { 1,1 }, { 1,1 }, { 1,0 }, { 0,0 },
		};
		for(uint32_t i = 0;i < 36;i++) {
			normal[i] = glm::normalize(normal[i]);
		}
		ms_pMeshCube = LEMesh::fromPositionNormalUV(vertices,normal,uv,indices);
	}
	// plane
	if(!ms_pMeshPlane) {
		ms_pMeshPlane = LEMesh::fromPositionUV({ {-10, 0, 10}, { -10,0,-10 }, { 10,0,-10 }, { 10,0,10 }},
                                               { {0,0},{0,1},{1,1},{1,0}},
                                               {0,1,2,2,3,0}
		);
	}
	int segmentSize = 100;
	// Sphere
	if(!ms_pMeshSphere) {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normal;
		std::vector<glm::vec2> uv;

		const float radius = 10.0f;
		std::vector<float> verticesValue;
		float angleSpan = 3.0f;
		for (float vAngle = 90; vAngle > -90; vAngle = vAngle - angleSpan) {
			for (float hAngle = 360; hAngle > 0; hAngle = hAngle - angleSpan) {
				double xozLength = radius * cos(glm::radians(vAngle));
				float x1         = (float)(xozLength * cos(glm::radians(hAngle)));
				float z1         = (float)(xozLength * sin(glm::radians(hAngle)));
				float y1         = (float)(radius * sin(glm::radians(vAngle)));

				xozLength        = radius * cos(glm::radians(vAngle - angleSpan));
				float x2         = (float)(xozLength * cos(glm::radians(hAngle)));
				float z2         = (float)(xozLength * sin(glm::radians(hAngle)));
				float y2         = (float)(radius * sin(glm::radians(vAngle - angleSpan)));

				xozLength        = radius * cos(glm::radians(vAngle - angleSpan));
				float x3         = (float)(xozLength * cos(glm::radians(hAngle - angleSpan)));
				float z3         = (float)(xozLength * sin(glm::radians(hAngle - angleSpan)));
				float y3         = (float)(radius * sin(glm::radians(vAngle - angleSpan)));

				xozLength        = radius * cos(glm::radians(vAngle));
				float x4         = (float)(xozLength * cos(glm::radians(hAngle - angleSpan)));
				float z4         = (float)(xozLength * sin(glm::radians(hAngle - angleSpan)));
				float y4         = (float)(radius * sin(glm::radians(vAngle)));

				verticesValue.push_back(x1); verticesValue.push_back(y1); verticesValue.push_back(z1);
				verticesValue.push_back(x2); verticesValue.push_back(y2); verticesValue.push_back(z2);
				verticesValue.push_back(x4); verticesValue.push_back(y4); verticesValue.push_back(z4);
				verticesValue.push_back(x4); verticesValue.push_back(y4); verticesValue.push_back(z4);
				verticesValue.push_back(x2); verticesValue.push_back(y2); verticesValue.push_back(z2);
				verticesValue.push_back(x3); verticesValue.push_back(y3); verticesValue.push_back(z3);
			}
		}
		auto generateTexCoor = [](int bw, int bh) {
			std::vector result(bw * bh * 6 * 2, 0.0f);
			const float sizew = 1.0f / bw;
			const float sizeh = 1.0f / bh;
			int index = 0;
			for (int i = 0; i < bh; i++) {
				for (int j = 0; j < bw; j++) {
					float s = j * sizew;
					float t = i * sizeh;
					result[index++] = s;
					result[index++] = t;
					result[index++] = s;
					result[index++] = t + sizeh;
					result[index++] = s + sizew;
					result[index++] = t;
					result[index++] = s + sizew;
					result[index++] = t;
					result[index++] = s;
					result[index++] = t + sizeh;
					result[index++] = s + sizew;
					result[index++] = t + sizeh;
				}
			}
			return result;
		};
		std::vector<float> texCoor = generateTexCoor((int)(360 / angleSpan),(int)(180 / angleSpan));
		uint32_t vertexCount = verticesValue.size() / 3;
		vertices.resize(vertexCount);
		uv.resize(vertexCount);
		normal.resize(vertexCount);
		uint32_t index = 0;
		uint32_t texIndex = 0;
		for (uint32_t i = 0; i < vertexCount; i++) {
			vertices[i].x = verticesValue[i * 3 + 0];
			vertices[i].y = verticesValue[i * 3 + 1];
			vertices[i].z = verticesValue[i * 3 + 2];
			uv[i].x       = texCoor[texIndex++];
			uv[i].y       = texCoor[texIndex++];
			normal[i].x   = verticesValue[i * 3 + 0] / radius;
			normal[i].y   = verticesValue[i * 3 + 1] / radius;
			normal[i].z   = verticesValue[i * 3 + 2] / radius;
		}
		ms_pMeshSphere = LEMesh::fromPositionNormalUV(vertices, normal,uv,{});
	}

	// cylinder
	std::vector<glm::vec3> cylinderVertices;
	std::vector<int>       cylinderIndices;

	cylinderVertices.emplace_back(0, 0, 1.0f);
	cylinderVertices.emplace_back(0, 0, -1.0f);
	for (int i = 0; i < segmentSize; i++) {
		cylinderVertices.emplace_back(glm::cos(2 * glm::pi<float>() / segmentSize * i), 
		                              1.f,
		                              glm::sin(2 * glm::pi<float>() / segmentSize * i));
	}
	for (int i = 0; i < segmentSize; i++) {
		cylinderVertices.emplace_back(glm::cos(2 * glm::pi<float>() / segmentSize * i), 
		                              -1.f,
		                              glm::sin(2 * glm::pi<float>() / segmentSize * i));
	}
	// top and bottom sides
	for (int i = 0; i < segmentSize; i++) {
		// top
		cylinderIndices.push_back(0); // center of top circle
		cylinderIndices.push_back(i);
		cylinderIndices.push_back((i+1)%segmentSize);
		// bottom
		cylinderIndices.push_back(1); // center of bottom circle
		cylinderIndices.push_back(i + segmentSize);
		cylinderIndices.push_back((i + 1) % segmentSize + segmentSize);
	}
	for(int i = 0;i < segmentSize;i++) {
		cylinderIndices.push_back(i);
		cylinderIndices.push_back(i+1);
		cylinderIndices.push_back(i+segmentSize);
		cylinderIndices.push_back(i+ segmentSize);
		cylinderIndices.push_back(i+1);
		cylinderIndices.push_back(i+1+ segmentSize);
	}
}
