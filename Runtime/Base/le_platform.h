/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_platform.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#pragma once

// WINDOWS
#if defined(_WIN32) || defined(_WIN64)
	#ifndef LE_PLATFORM_WINDOWS
		#define LE_PLATFORM_WINDOWS
	#endif // !LE_PLATFORM_WINDOWS
#endif
// ANDROID
#if defined(__ANDROID__)
	#ifndef LE_PLATFORM_ANDROID
		#define LE_PLATFORM_ANDROID
	#endif // !LE_PLATFORM_ANDROID
// IOS or MAC
#elif defined(__APPLE__)
	// IOS
	#if defined(TARGET_OS_IPHONE)
		#ifndef LE_PLATFORM_IOS
			#define LE_PLATFORM_IOS
		#endif // !LE_PLATFORM_IOS
	#elif defined(TARGET_OS_MAC)
		#ifndef LE_PLATFORM_MAC
			#define LE_PLATFORM_MAC
		#endif // !LE_PLATFORM_IOS
	#else
		#error Unsupported Apple Device
	#endif
#endif
// LINUX
#if defined(__linux)
	#ifndef LE_PLATFORM_LINUX
		#define LE_PLATFORM_LINUX
	#endif // !LE_PLATFORM_LINUX
#endif 

// DEBUG or RELEASE
#if defined(LE_PLATFORM_WINDOWS)
	#if defined(_DEBUG)
		#ifndef LE_DEBUG
			#define LE_DEBUG
		#endif //!LE_DEBUG
	#else 
		#ifndef LE_RELEASE
			#define LE_RELEASE
		#endif //!LE_RELEASE
	#endif
#endif // !LE_PLATFORM_WINDOWS



	
	
