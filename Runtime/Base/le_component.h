#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_component.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_object.h"

// STL Includes
#include <vector>
#include <string>

class LEEntity;

#define LE_COMPONENT_NAME(componentName) \
	public:\
		virtual std::string& getComponentName() const { \
			static std::string componentNameStr(#componentName); \
			return componentNameStr; \
		}

class LEComponent:public LEObject {
public:
	enum ComponentFLag {
		/* one entity can only contain one component with same type */
		CF_Exclusive         = 1,  
		/* whether multiple components can be edited in property editor in the same time */
		CF_MultiEditable     = 2,  
		CF_Enabled           = 4,
		/* we need to differentiate two types of Component: build-in components and script components(C#) */
		CF_IS_BUILD_IN       = 8,  
		CF_IS_NOT_DELETEABLE = 16,
		CF_ComponentFLag_MAX = 0xffffffff
	};
protected:
	int                  m_flags;
	LEEntity*            m_pAttachedEntity;
public:
	                     LEComponent();

	virtual              ~LEComponent();

	virtual std::string& getComponentName() const = 0;

	void                 destorySelf();

	void                 destorySelfLater(float delayTimeMs);
		                 
	void                 setEnabled(bool value);
		                 
	bool                 isEnabled() const;
				         
	bool                 isDeletable() const;
				         
	bool                 isBuildIn() const;
				         
	LEEntity*            getAttachedEntity() const;
				         
	virtual bool         isRenderer() const;

#if defined(LE_EDITOR)
	virtual void         onPropertyEditorGUI();
#endif // !LE_EDITOR
	friend class LEEntity;
};
