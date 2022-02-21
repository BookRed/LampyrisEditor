#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_entity_layer_manager.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_singleton.h"
#include <vector>

struct LEEntityLayerInfo {
	std::string layerName;
	int         layerIndex;
};

typedef int LELayer_t;

class LEEntityLayerManager:public Singleton<LEEntityLayerManager> {
private:
	constexpr static int           ms_maxLayerCount = 32;
	std::vector<LEEntityLayerInfo> m_layers;

	void loadBuildInLayers() {
		this->m_layers[0] = LEEntityLayerInfo{ "Default",0 };
		this->m_layers[1] = LEEntityLayerInfo{ "Ignore Raycast",1};
		this->m_layers[2] = LEEntityLayerInfo{ "UI",2 };
	}
public:
#if defined(LE_EDITOR)
	void setCustomLayersInEditor(std::vector<LEEntityLayerInfo>& layerInfo) {
		this->m_layers = std::move(layerInfo);
	}

#endif // !LE_EDITOR
	LEEntityLayerManager() {
		this->m_layers.resize(ms_maxLayerCount);
		this->loadBuildInLayers();
	}

	virtual ~LEEntityLayerManager() {
		
	}

	const std::vector<LEEntityLayerInfo>& getLayerInfo() const {
		return this->m_layers;
	}
};
