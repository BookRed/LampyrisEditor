#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_time.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_TIME_H
#define LE_TIME_H

// LE Includes
#include "../Base/le_global.h"

// STL Includes
#include <numeric>
#if defined(LE_PLATFORM_WINDOWS) 
class LEWindowsTimeCounter {
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startCounter;
	double        m_passTimeSecond;
public:
	LEWindowsTimeCounter():
		m_frequency(),
		m_passTimeSecond(std::numeric_limits<double>::max()),
m_startCounter() {
		::QueryPerformanceFrequency(&this->m_frequency);
	}

	virtual ~LEWindowsTimeCounter() {}

	void start() {
		::QueryPerformanceCounter(&this->m_startCounter);
	}

	void end() {
		LARGE_INTEGER currentCounter;
		::QueryPerformanceCounter(&currentCounter);
		this->m_passTimeSecond =  double(currentCounter.LowPart - this->m_startCounter.LowPart) / (double)m_frequency.LowPart;
	}

	double getPassedTimeMillionSecond() const {
		return this->m_passTimeSecond*1000;
	}

	double getPassedTimeSecond() const {
		return this->m_passTimeSecond;
	}

	void reset() {
		this->m_startCounter = LARGE_INTEGER();
		this->m_passTimeSecond = std::numeric_limits<double>::max();
	}
};
typedef LEWindowsTimeCounter LETimeCounter;
#endif // !LE_PLATFORM_WINDOWS
#endif // !LE_TIME_H