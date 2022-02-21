#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_new_delete_override.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_NEW_DELETE_OVERRIDE_H
#define LE_NEW_DELETE_OVERRIDE_H

// STD Includes
#include <memory>
#include <new>
#include <iostream>

void* operator new(std::size_t size,
	               const char* file,
	               const char* func,
	               int lineno) noexcept;

#if defined(new)
#undef new
#endif // !new

// #define new new(__FILE__,__FUNC__,__LINE__)

#endif // !LE_NEW_DELETE_OVERRIDE_H