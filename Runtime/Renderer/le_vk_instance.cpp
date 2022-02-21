/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Renderer
 * \File:    le_vk_instance.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_vk_instance.h"

void LEVkInstance::createGlobalInstance() {
	// the extensions of Vulkan instance
	std::vector<const char*> vkInstanceExtensionNames;
	vkInstanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);
	vkInstanceExtensionNames.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

	LEConfig* config = LEConfigManager::getInstance()->getConfig("app_config");
	LE_ASSERT(config != nullptr);
	// Create the instance of Vulkan Application
	std::string applicationName = config->readString("Base", "AppName");
	std::string engineName      = config->readString("Vulkan", "EngineName");
	VkApplicationInfo app_info  = {};
	app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext              = NULL;
	app_info.pApplicationName   = applicationName.c_str();
	app_info.applicationVersion = config->readInt("Base", "AppVersion");
	app_info.pEngineName        = engineName.c_str();
	app_info.engineVersion      = config->readInt("Vulkan", "EngineVersion");
	app_info.apiVersion         = VK_API_VERSION_1_2;

	/* the names of layer that we except to launch*/
	std::vector<const char*> exceptedLayerNames;
	exceptedLayerNames.push_back("VK_LAYER_KHRONOS_validation");
	exceptedLayerNames.push_back("VK_LAYER_LUNARG_override");
	exceptedLayerNames.push_back("VK_LAYER_LUNARG_device_simulation");
	exceptedLayerNames.push_back("VK_LAYER_LUNARG_monitor");
	exceptedLayerNames.push_back("VK_LAYER_LUNARG_screenshot");

	uint32_t layerCount = -1; // the count of layer 
	//try to get the count of layer
	::vkEnumerateInstanceLayerProperties(&layerCount, NULL);

	//declare a vector for saving the VkLayerProperties,and then resize it to layerCount
	std::vector<VkLayerProperties> vkLayerProperties;
	vkLayerProperties.resize(layerCount);

	// get all vkLayerProperties
	vkEnumerateInstanceLayerProperties(&layerCount, vkLayerProperties.data());

	// a vector which saves the layer that support to be verified
	std::vector<std::string*> layerNames;

	// a vector which saves the extensions that support to be verified
	std::vector<std::string*> extensionNames;

	// and then traverse all VklayerProperties
	for (int i = 0; i < vkLayerProperties.size(); i++) {
		// get current layerProperty
		VkLayerProperties layerProperty = vkLayerProperties[i];

		// a flag represents whether the layer is need or not
		bool flag = false;

		// figure out whether current layerProperty is in exceptedLayer;
		for (auto s : exceptedLayerNames) {
			if (strcmp(s, layerProperty.layerName) == 0)
			{
				flag = true;
				break;
			}
		}
		//add the current layer name to the layerNames
		if (flag) {
			layerNames.push_back(new std::string(layerProperty.layerName));
		}

		// the count of extension's property corresponding to current layer
		uint32_t propertyCount;
		vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &propertyCount, NULL);

		// a vector to save extension's property
		std::vector<VkExtensionProperties> propertiesList;

		// resize it to propertyCount
		propertiesList.resize(propertyCount);

		vkEnumerateInstanceExtensionProperties(layerProperty.layerName, &propertyCount, propertiesList.data());
		for (auto ep : propertiesList) {
			// if the validation layer is need
			if (flag) {
				// figure out whether current extension's name is in extensionName;
				bool contains = false;
				for (auto s : exceptedLayerNames) {
					if (strcmp(ep.extensionName, s) == 0) {
						contains = true;
						break;
					}
				}
				// if not contain, add it to extensionName 
				if (!contains) {
					extensionNames.push_back(new std::string(ep.extensionName));
				}
			}
		}
	}
	// add needed extensions to extensionNames
	for (auto s : extensionNames) {
		vkInstanceExtensionNames.push_back((*s).c_str());
	}
	// clear exceptedLayerNames
	exceptedLayerNames.clear();

	// add the layer that support to be verified to exceptedLayerNames
	for (auto s : layerNames) {
		exceptedLayerNames.push_back((*s).c_str());
	}

	VkValidationFeatureEnableEXT enabled[]  = { VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT };
	VkValidationFeaturesEXT      features   = {};
	features.sType                          = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	features.pNext                          = nullptr;
	features.disabledValidationFeatureCount = 0;
	features.enabledValidationFeatureCount  = 1;
	features.pDisabledValidationFeatures    = nullptr;
	features.pEnabledValidationFeatures     = enabled;

	VkInstanceCreateInfo instanceInfo    = {};
	instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;  // the type of struct
	instanceInfo.pNext                   = &features;								// pointer of user data
	instanceInfo.pApplicationInfo        = &app_info;								// bind app info struct
	instanceInfo.enabledExtensionCount   = vkInstanceExtensionNames.size();	        // count of extensions
	instanceInfo.ppEnabledExtensionNames = vkInstanceExtensionNames.data();         // pointer of extensions
	instanceInfo.enabledLayerCount       = exceptedLayerNames.size();			    // count of layer
	instanceInfo.ppEnabledLayerNames     = exceptedLayerNames.data();			    // pointer of layer

	LE_ASSERT(vkCreateInstance(&instanceInfo, NULL, &vkVariableManager->vkInstance) == VK_SUCCESS);
}

void LEVkInstance::destoryGlobalInstance() {
	vkDestroyInstance(vkVariableManager->vkInstance, nullptr);
}
