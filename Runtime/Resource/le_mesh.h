#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_mesh.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// STL Includes
#include <array>

// LE Includes
#include "../Base/le_assert.h"
#include "../Renderer/le_vk_variable_manager.h"
#include "le_resource.h"
#include "../Math/le_AABB.h"

// GLM Includes
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

class LEVkBuffer;
class LEVertexBuffer {
private:
	int m_vertexCount;
public:
	enum VertexFormat {
		VF_POSITION,              // R G B
		VF_COLOR,                 // R G B A
		VF_TEXCOOR,               // R G
		VF_TEXCOOR1 = VF_TEXCOOR, // R G
		VF_TEXCOOR2,              // R G
		VF_TEXCOOR3,              // R G
		VF_TEXCOOR4,              // R G
		VF_TANGENT,               // R G B A
		VF_BINORMAL,              // R G B A
		VF_NORMAL,                // R G B
		VF_MAX		           
	};
	static constexpr uint32_t ms_vertexFormatCount = VF_MAX;

private:
	LEVkBuffer*                m_buffer;
	std::array<short, VF_MAX>  m_formatOffset;
	int                        m_byteCountPerVertex;
	static constexpr int s_byteCountMap[] = { 
		sizeof(glm::vec3),sizeof(glm::vec4),sizeof(glm::vec2),
		sizeof(glm::vec2),sizeof(glm::vec2),sizeof(glm::vec2),
		sizeof(glm::vec4),sizeof(glm::vec4),sizeof(glm::vec3)};

public:
	LEVertexBuffer(std::vector<VertexFormat> formats,int vertexCount);

	~LEVertexBuffer();

	bool hasFormat(VertexFormat format) const;

	int getVertexCount() const;

	void setVertexValueAt(unsigned char* data, int byteCountPerVertex, int index, VertexFormat format) const;

	void setVertexValue(unsigned char* data, int byteCountPerVertex, VertexFormat format) const;

	void setPositionAt(const glm::vec3& position,int index) const;

	void setPosition(const std::vector<glm::vec3>& position) const;

	void setColorAt(const glm::vec4& color, int index) const;

	void setColor(const std::vector<glm::vec4>& color) const;

	void setTexCoorAt(const glm::vec2& texCoor, int index) const;

	void setTexCoor(const std::vector<glm::vec2>& uv) const;

	void setNormal(const std::vector<glm::vec3>& normals) const;

	LEVkBuffer* get();

	uint32_t getFormatOffset(VertexFormat format) const;
};

class LEIndexBuffer {
private:
	LEVkBuffer*     m_buffer;
	unsigned int    m_indexCount;
public:
	LEIndexBuffer(const std::vector<int>& indices);

	int getIndexCount() const;

	void setIndexData(const std::vector<int>& indices);

	LEVkBuffer* get();
};

class LEMesh:public LEObject,public LEResource {
private:
	LEVertexBuffer* m_vertexBuffer { nullptr};
	LEIndexBuffer*  m_indexBuffer { nullptr};

	LEAABB*         m_pAABB {nullptr};
	LEMesh();

	void            calculateAABB(const std::vector<glm::vec3>& position);
public:

	~LEMesh();

	LEVertexBuffer* getVertexBuffer();

	LEIndexBuffer* getIndexBuffer();

	static LEMesh* fromPosition(const std::vector<glm::vec3> &position,
		                        const std::vector<int>& indices);

	// indices auto-generated
	static LEMesh* fromPosition(const std::vector<glm::vec3>& position);

	static LEMesh* fromPositionColor(const std::vector<glm::vec3>& position,
									 const std::vector<glm::vec4>  color,    
		                             const std::vector<int>& indices);

	static LEMesh* fromPositionUV(const std::vector<glm::vec3>& position,
							      const std::vector<glm::vec2>  uv,   
		                          const std::vector<int>& indices);

	static LEMesh* fromPositionNormalUV(const std::vector<glm::vec3>& position,
							            const std::vector<glm::vec3>  normal,   
							            const std::vector<glm::vec2>  uv,   
		                                const std::vector<int>& indices);

	static std::vector<glm::vec3> calculateNormal(const std::vector<glm::vec3>& position, const std::vector<int>& indices);

	static LEMesh* ms_pMeshCube;

	static LEMesh* ms_pMeshPlane;

	static LEMesh* ms_pMeshSphere;

	static LEMesh* getCubeMesh();

	static LEMesh* getPlaneMesh();

	static LEMesh* getSphereMesh();

	static void    createBuildInResources();

	ResourceType getType() const override;

	LEAABB* getAABB();

	void onLoaded() override;

	void onUnloaded() override;
};

