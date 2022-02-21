#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module: Base
 * \File:   LE_global.h
 * \Author: BookRed
 * \Email:  390031942@qq,com
*/

#ifndef LE_GLOBAL_H
#define LE_GLOBAL_H

#ifdef _LAMPYRIS_ENGINE_PROJECT_EXPORT
#  define LE_API_EXPORT __declspec(dllexport)
#else
#  define LE_API_EXPORT __declspec(dllimport)
#endif //!_LAMPYRIS_ENGINE_PROJECT_EXPORT

#define LE_PY_EXPORT LE_API_EXPORT

#ifndef LE_NAMESPACE_MACRO
#define LE_NAMESPACE_MACRO

#define LE_BASE_NAMESPACE_BEGIN namespace Lampyris { 
#define LE_BASE_NAMESPACE_END }

#define LE_NAMESPACE_BEGIN(name) namespace Lampyris { namespace name {
#define LE_NAMESPACE_END         }}

#define LE_DETAIL_NAMESPACE_BEGIN(name) namespace Lampyris { namespace Detail { namespace name {
#define LE_DETAIL_NAMESPACE_END         }}}

#define LE_USING_NAMESPACE(name) using namespace Lampyris::name;
#define LE_USING_DETAIL_NAMESPACE(name) using namespace Lampyris::Detail::name;

#define LE_NONCOPYABLE(className) className(const className&) = delete;\
                                  className & operator = (const className&) = delete;

#define LE_DECLARE_FRIEND_CLASS(className) friend class className;
#define LE_INLINE  inline 

#define extend     public
#define implement  public
#endif //!LE_NAMESPACE_MACRO

// Includes
#include "le_typedef.h"
#include "le_platform.h"
#include "le_safe_release.h"
#include "le_new_delete_override.h"

#ifdef LE_PLATFORM_WINDOWS
    #include <Windows.h>
    #define LE_FORCEINLINE __forceinline

    #ifdef _MSVC_LANG
        #if _MSVC_LANG < 201705L
            # error The C++ Language Standard of MSVC is not supported C++ 20,set it in [Debug]/[Debug Properties]/[C++ Language Standard]
        #endif // !_MSVC_LANG < 201705L
        #pragma warning(disable:4244) 
        #pragma warning(disable:4267)
        #pragma warning(disable:4018)
        #pragma warning(disable:4302)
        #pragma warning(disable:4312)
        #pragma warning(disable:4996)
        #pragma warning(disable:4099)
    #endif // !_MSVC_LANG
#endif // !LE_PLATFORM_WINDOWS

#if defined(interface)
#undef interface
#endif

#define interface  class
#endif //!LE_GLOBAL_H