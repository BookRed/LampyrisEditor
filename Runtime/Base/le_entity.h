#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_entity.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_component.h"
#include "le_entity_layer_manager.h"

#ifndef LE_ENTITY_H
#define LE_ENTITY_H

class LEPrimitiveRenderer;
class LETransform;

#if defined(LE_EDITOR)
class QTreeWidgetItem;
#endif

// the entity in the game world
class LEEntity:public LEObject {
private:
	bool                             m_active;
	LELayer_t                        m_layer;
	std::vector<LEEntity*>           m_children;
	std::vector<LEComponent*>        m_pComponents;
	std::string                      m_name;
	LEEntity*                        m_pParent;
	LETransform*                     m_pTransform;

	void                             removeChild(LEEntity* pChild);
public:			                     
	                                 LEEntity(const std::string& name);
				                     
	                                ~LEEntity();
				                     
	void                             addComponent(LEComponent* pComponent);
				                     
	void                             removeComponent(LEComponent* pComponent);
				                     
	LEComponent*                     getComponent(const std::string& name);
				                     
	LETransform*                     getTransform() const;
				                     
	void                             setParent(LEEntity* pParent);
				                     
	LEEntity*                        getParent() const;
				                     
	LEEntity*                        findFirstByName();
				                     
	LEEntity*                        findByName();
				                     
	void                             destorySelf();
		                             
	void                             destorySelfLater(float delayTimeMs);
		                             
	bool                             isActive() const;
		                             
	void                             setActive(bool value);

	const std::vector<LEComponent*>& getAllComponents();

	const std::vector<LEEntity*>&    children();

	void                             setName(const std::string& name);
					                 
	const std::string&               getName() const;

#if defined(LE_EDITOR)
	QTreeWidgetItem*                 pTreeItem;
#endif 
};

#endif // !LE_ENTITY_H

