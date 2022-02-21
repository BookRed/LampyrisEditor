/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Utility
 * \File:    le_system_info.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_system_info.h"
#include "../Renderer/le_vk_variable_manager.h"

// STD Includes
#include<iomanip>

LESystemInfoStruct LESystemInfo::ms_systemInfo;
bool LESystemInfo::queryNvidiaGPUInfo() {
	/* try to query NVIDIA's GPU */
	NvU32				driveVersion;
	NvAPI_ShortString	driveBranch;
	NvU32               gpuCount = 0;
	NvPhysicalGpuHandle gpuHandles[NVAPI_MAX_PHYSICAL_GPUS];
	NvAPI_Status status = NvAPI_Initialize();
	if (status != NvAPI_Status::NVAPI_OK) {
		// [Info] : init Nvidia API failed;
		return false;
	}
	NvAPI_EnumPhysicalGPUs(gpuHandles, &gpuCount);
	if (gpuCount <= 0) {
		// [Info] : no NVIDIA's GPU found
		return false;
	}
	status = NvAPI_SYS_GetDriverAndBranchVersion(&driveVersion, driveBranch);
	if (status != NvAPI_Status::NVAPI_OK) {
		// [Info] : get driver version failed;
		return false;
	}
	NvAPI_ShortString gpuName = "";
	status = NvAPI_GPU_GetFullName(gpuHandles[0], gpuName);
	ms_systemInfo.gpuName = std::string("NVIDIA ") + gpuName;
	ms_systemInfo.gpuDriverVersion = (std::stringstream()
		                              <<(driveVersion / 100.0f) 
		                              << std::setiosflags(std::ios::fixed) 
		                              << std::setprecision(2)).str();
	NvAPI_Unload();
	return true;
}

bool LESystemInfo::queryAMDGPUInfo() {
	AGSContext* agsContext  = nullptr;
	AGSGPUInfo gpuInfo      = {};
	AGSConfiguration config = {};

	// init AGS Library
	if (::agsInitialize(AGS_MAKE_VERSION(AMD_AGS_VERSION_MAJOR,
		AMD_AGS_VERSION_MINOR,
		AMD_AGS_VERSION_PATCH),
		&config, &agsContext, &gpuInfo) == AGS_SUCCESS) {
		if (gpuInfo.devices != nullptr) {
			ms_systemInfo.gpuName = gpuInfo.devices[0].adapterString;
			ms_systemInfo.gpuDriverVersion = gpuInfo.driverVersion;
		}
		else {
			// [Info] : no AMD's GPU found
			::agsDeInitialize(agsContext);
			return false;
		}
		// cleanup AGS Library
		::agsDeInitialize(agsContext);
		return true;
	}
	else {
		return false;
	}
}

void LESystemInfo::queryGPUInfo() {
	bool amdGPUFound = queryAMDGPUInfo();
	bool nvidiaGPUFound = queryNvidiaGPUInfo();

	if (!amdGPUFound && !nvidiaGPUFound) {
		// [Fatal Error] : Unsupported GPU
		// LEApplication::getInstance()->quit();
	}
}

void LESystemInfo::querySystemInfo() {
	if (ms_systemInfo.isSystemInfoAcquired) {
		return;
	}
	queryCPUInfo();
	queryGPUInfo();
	if(0)
	{	/*
		::vkEnumeratePhysicalDevices(vkVariableManager->vkInstance,
			&ms_systemInfo.gpuCount,
			nullptr);
		vkVariableManager->gpuCount = ms_systemInfo.gpuCount;
		::vkEnumeratePhysicalDevices(vkVariableManager->vkInstance,
			&ms_systemInfo.gpuCount,
			vkVariableManager->gpus);
		for (int i = 0; i < vkVariableManager->gpuCount; i++) {
			::vkGetPhysicalDeviceProperties(vkVariableManager->gpus[i], &vkVariableManager->gpuProperties[i]);
			::vkGetPhysicalDeviceMemoryProperties(vkVariableManager->gpus[i], &vkVariableManager->gpuMemoryProperties[i]);
			std::stringstream gpuInfoStringStream;
			gpuInfoStringStream << vkVariableManager->gpuProperties[i].deviceName
				<< " "
				<< vkVariableManager->gpuMemoryProperties[i].memoryHeaps[0].size / 1048576 << " MB";
			// ms_systemInfo.gpuName.emplace_back(gpuInfoStringStream.str());
			// ms_systemInfo.gpuDriverVersions.emplace_back(vkVariableManager->gpuProperties[i].driverVersion);
		}
		*/
	}
	queryMemoryInfo();
}

const std::string& LESystemInfo::getCPUName() {
	return ms_systemInfo.cpuNameString;
}

const std::string& LESystemInfo::getDeviceName() {
	static std::string str(256, '\0');
	if (str[0] == '\0') {
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (::gethostname(str.data(), 256) != 0) {
			str.assign("unknown device");
		}
		WSACleanup();
	}
	return str;
}

const std::string& LESystemInfo::getOSInfoString() {
	return ms_systemInfo.systemInfoString;
}

bool LESystemInfo::isOSSupported() {
#if defined(LE_PLATFORM_WINDOWS)
	return ms_systemInfo.isWindows10OrLater;
#endif // !LE_PLATFORM_WINDOWS
}

bool LESystemInfo::isGPUSupported() {
	return true;
}

bool LESystemInfo::isCPUSupported() {
	return true;
}

bool LESystemInfo::isGPUDriverVersionSatisfied() {
	return true;
}

float LESystemInfo::getMemorySizeGB() {
	return 0;
}

const std::string& LESystemInfo::getGPUName() {
	return ms_systemInfo.gpuName;
}

const std::string& LESystemInfo::getGPUDriverVersion() {
	return ms_systemInfo.gpuDriverVersion;
}

float LESystemInfo::getGPUCoreFrequency() {
	return ms_systemInfo.cpuFrequency;
}

float LESystemInfo::getGPUMemoryFrequency() {
	return 0;
}

int LESystemInfo::getDiskCount() {
	return 0;
}
