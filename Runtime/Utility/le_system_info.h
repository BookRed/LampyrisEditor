#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_system_info.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SYSTEM_INFO_H
#define LE_SYSTEM_INFO_H

// LE Includes
#include "../Base/le_global.h"
#include "le_string_convert.h"

#if defined(LE_PLATFORM_WINDOWS)
#include <VersionHelpers.h>
#pragma comment(lib,"Ws2_32.lib")
#endif // !LE_PLATFORM_WINDOWS

// STL Includes
#include <string>
#include <sstream>
#include <vector>

// Vulkan Includes
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#pragma comment(lib,"vulkan-1.lib")

// NVIDIA Includes
#include <nvidia/nvapi.h>
#pragma comment(lib,"nvapi64.lib")

// AMD Includes
#define AGS_FORWARD_DECLARE_DIRECTX_TYPES
#include <AMD/amd_ags.h>
#if defined(LE_DEBUG)
  #pragma comment(lib,"amd_ags_x64_2019_MDd.lib")
#else 
  #pragma comment(lib,"amd_ags_x64_2019_MD.lib")

#endif // !LE_DEBUG

// Reference
// [1]:https://blog.csdn.net/qq_34227896/article/details/82869445
// [2]:https://blog.csdn.net/justFWD/article/details/44856277
// [3]:https://blog.csdn.net/weixin_35457696/article/details/119096145
// [4]:https://blog.csdn.net/fduffyyg/article/details/86505051
// [5]:https://github.com/GPUOpen-LibrariesAndSDKs/AGS_SDK/blob/master/ags_sample/src/AGSSample.cpp

struct LESystemInfoStruct {
	/* operation system info*/
	bool isSystemInfoAcquired = false;
#if defined(LE_PLATFORM_WINDOWS)
	bool isWindows10OrLater = false;;
#endif // !LE_PLATFORM_WINDOWS
	std::string systemInfoString;

	/* CPU info*/
	std::string  cpuNameString;
	unsigned int cpuCoreCount;
	float        cpuFrequency;

	/* GPU info*/
	std::string  gpuName;
	std::string  gpuDriverVersion;

	// Memory Info
	float        memorySize;
};
class LESystemInfo {
private:
	static LESystemInfoStruct ms_systemInfo;

	static bool queryCPUInfo() {
		#if defined(LE_PLATFORM_WINDOWS)
		HKEY  hKey;
		/* query Operation System information */
		LONG  nRet = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE,                                 /*key*/
									 L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", /*lpSubKey*/
									 0,                                                  /*ulOptions*/
									 KEY_ALL_ACCESS,                                     /*samDesired*/
									 &hKey                                               /*phkResult*/);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query system info failed!
			return false;
		}
		WCHAR szCurrentVersion[100] = { 0 };
		WCHAR szCurrentBuild[100]   = { 0 };
		WCHAR szProductName[100]    = { 0 };
		WCHAR szEditionID[100]      = { 0 };
		DWORD dwSize                = 100;
		DWORD type                  = 0;
		nRet = ::RegQueryValueExW(hKey,
								  L"CurrentVersion",
								  NULL,
								  &type,
								  (BYTE*)szCurrentVersion,
								  &dwSize);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query Operation System 'CurrentVersion' failed!
			return false;
		}
		// Lampyris Engine only supports Windows 10 or later
		if (::wcscmp(szCurrentVersion, L"6.3") >= 0) {
			ms_systemInfo.isWindows10OrLater = true;
		}
		else {
			ms_systemInfo.isWindows10OrLater = false;
		}
		dwSize = 100;
		nRet = ::RegQueryValueExW(hKey,
								  L"CurrentBuild",
								  NULL,
								  &type,
								  (BYTE*)szCurrentBuild,
								  &dwSize);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query Operation System 'CurrentBuild' failed!
			return false;
		}
		dwSize = 100;
		nRet = ::RegQueryValueExW(hKey,
								  L"EditionID",
								  NULL,
								  &type,
								  (BYTE*)szEditionID,
								  &dwSize);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query Operation System 'EditionID' failed!
			return false;
		}
		auto systemInfoWString = (std::wstringstream()
			<< L"Microsoft Windows 10"
			<< szProductName
			<< " " << szEditionID << " Edition"
			<< " (" << szCurrentVersion << ")"
			<< " Build " << szCurrentBuild).str();

		ms_systemInfo.systemInfoString = le_wchar_to_char(systemInfoWString);

		/* query CPU information */
		nRet = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, /*key*/
							   L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor", /*lpSubKey*/
							   0,/*ulOptions*/
							   KEY_ALL_ACCESS, /*samDesired*/
							   &hKey /*phkResult*/);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query CPU Information failed!
			return false;
		}
		nRet = ::RegQueryInfoKey(hKey, NULL, NULL, NULL,
			(LPDWORD) & (ms_systemInfo.cpuCoreCount),
			NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query 'cpuCoreCount' failed!
			return false;
		}
		nRet = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, /*key*/
							   L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", /*lpSubKey*/
							   0,/*ulOptions*/
							   KEY_ALL_ACCESS, /*samDesired*/
							   &hKey /*phkResult*/);
		WCHAR szProcessorNameString[100] = { 0 };
		dwSize = 100;
		nRet = ::RegQueryValueExW(hKey,
								  L"ProcessorNameString",
								  NULL,
								  &type,
								  (BYTE*)szProcessorNameString,
								  &dwSize);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query 'cpuName' failed!
			return false;
		}
		dwSize = 100;
		nRet = ::RegQueryValueExW(hKey,
								  L"~MHz",
								  NULL,
								  &type,
								  (BYTE*)&ms_systemInfo.cpuFrequency,
								  &dwSize);
		if (nRet != ERROR_SUCCESS) {
			// [Fatal Error]: Query 'cpFrequency' failed!
			return false;
		}
		auto cpuNameWString = (std::wstringstream()
			<< szProcessorNameString << L" "
			<< ms_systemInfo.cpuCoreCount << " Core(s) ").str();
		ms_systemInfo.cpuNameString = le_wchar_to_char(cpuNameWString);
		return true;
	#endif
	}
	static bool queryNvidiaGPUInfo();

	static bool queryAMDGPUInfo();

	static void queryGPUInfo();

	static void queryMemoryInfo() {
		constexpr static int s_GBYTES  = 1073741824;
		constexpr static int s_MBYTES  = 1048576;
		constexpr static int s_DKBYTES = 1024.0;
	#if defined(LE_PLATFORM_WINDOWS)
		MEMORYSTATUSEX memoryStatusEx;
		memoryStatusEx.dwLength = sizeof(memoryStatusEx);
		if (:: GlobalMemoryStatusEx(&memoryStatusEx)){
			unsigned long long total       = 0;
			unsigned long long remainTotal = 0;
			unsigned long long avl         = 0;
			unsigned long long remainAvl   = 0;
			double decimalTotal            = 0;
			double decimalAvl              = 0;
			remainTotal  = memoryStatusEx.ullTotalPhys % s_GBYTES;
			total        = memoryStatusEx.ullTotalPhys / s_GBYTES;
			avl          = memoryStatusEx.ullAvailPhys / s_GBYTES;
			remainAvl    = memoryStatusEx.ullAvailPhys % s_GBYTES;
			if (remainTotal > 0) {
				decimalTotal = (remainTotal / s_MBYTES) / s_DKBYTES;
			}
			if (remainAvl > 0) {
				decimalAvl = (remainTotal / s_MBYTES) / s_DKBYTES;
			}
			decimalTotal += (double)total;
			decimalAvl   += (double)avl;
			
			ms_systemInfo.memorySize = decimalTotal;
	#endif // !LE_PLATFORM_WINDOWS
		}
	}
public:
	static void querySystemInfo();

	static const std::string& getCPUName();

	static const std::string& getDeviceName();

	static const std::string& getOSInfoString();

	static bool isOSSupported();

	static bool isGPUSupported();

	static bool isCPUSupported();

	static bool isGPUDriverVersionSatisfied();

	static float getMemorySizeGB();

	static const std::string& getGPUName();

	static const std::string& getGPUDriverVersion();

	static float getGPUCoreFrequency();

	static float getGPUMemoryFrequency();

	static int getDiskCount();

};

#endif // !LE_SYSTEM_INFO_H
