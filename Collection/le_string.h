#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Collection
 * \File:    le_string.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_STRING_H
#define LE_STRING_H

// LE Includes
#include "../Common/le_global.h"
#include "le_KMP.h"
#include "le_vector.h"
#include "le_string_convert.h"
#include "../Debug/le_assert.h"

LE_NAMESPACE_BEGIN(Collection)

class LEString :public LECollectionBase<LEWChar> {
private:
	const constexpr static uint32_t ms_BufferSize = 32;
	LEVector<LEString> splitImpl(const LEVector<LEWChar>& chars, uint32_t index) {
		LEVector<LEString> result;
		LEString currentStr;
		for (uint32_t i = 0; i < this->m_length; i++) {
			uint32_t j = 0;
			for (; j < chars.getLength(); j++) {
				if (this->m_pData[i] == chars[j]) {
					break;
				}
			}
			if (j == chars.getLength()) {
				currentStr += this->m_pData[i];
			}
			else {
				if (!currentStr.isEmpty()) {
					result.appendBack(currentStr);
				}
			}
		}
		if (result.isEmpty()) {
			result.appendBack(*this);
		}
		return result;
	}
	inline LEBool isOverFlow(uint32_t expectSize) {
		return expectSize > this->m_capacity;
	}
public:
	LEString():LECollectionBase<LEWChar>() {
	
	}

	LEString(const LEChar* str) {
		std::wstring wstr = le_char_to_wchar(str);
		new (this)LEString(wstr.c_str());
	}

	LEString(const LEWChar* str) {
		if (str == nullptr) {
			this->m_pData = new LEWChar[LE_COLLECTION_MIMIMUM_CAPACITY];
			this->m_pData[0] = L'\0';
			this->m_capacity = LE_COLLECTION_MIMIMUM_CAPACITY - 1;
			this->m_length = 0;
		}
		else {
			this->m_length = ::wcslen(str);
			this->m_capacity = getMax(LE_COLLECTION_MIMIMUM_CAPACITY,getNextPow2(this->m_length + 1));
			this->m_pData = new LEWChar[this->m_capacity];
			::memset(this->m_pData, 0, sizeof(LEWChar) * m_capacity);
			::wcsncpy(this->m_pData, str, this->m_length);
			// reserve for L'\0' 
			this->m_capacity--;
		}
	}

	// Copy Constructor
	LEString(const LEString& other) {
		this->m_pData = new LEWChar[other.m_capacity];
		this->m_length = other.m_length;
		this->m_capacity = other.m_capacity;
		::memset(this->m_pData, 0, sizeof(LEWChar) * this->m_capacity);
		::wcsncpy(this->m_pData, other.m_pData, other.m_length);
	}
	// Move Constructor
	LEString(LEString&& other) noexcept{
		this->swap(other);
	}

	 // Destructor
	~LEString() {

	}

	// override operator
	LEString& operator =(const LEString& val) {
		if (this == &val) {
			return *this;
		}
		m_pData = new LEWChar[::wcslen(val.m_pData) + 1];
		::wcscpy(m_pData, val.m_pData);
		return *this;
	}

	LEString& operator + (const LEString& str) {
		int expectedSize = str.m_length + this->m_length;
		LEWChar* data = nullptr;
		if (expectedSize > this->m_capacity - 1) {
			LEWChar allocateSize = getNextPow2(expectedSize + 1);
			data = new LEWChar[allocateSize]{ L'\0' };
			::memcpy(data, this->m_pData, sizeof(LEWChar) * allocateSize);
			::wcscat(data, str.m_pData);
			LE_SAFE_RELEASE_ARRAY(this->m_pData);
			this->m_pData = data;
			this->m_capacity = allocateSize-1;
		}
		else {
			::wcscat(this->m_pData, str.m_pData);
		}
		this->m_length = expectedSize;
		return *this;
	}

	LEString& operator + (int value) {
		LEWChar buffer[ms_BufferSize] = { L'\0' };
		::wsprintf(buffer, L"%d", value);
		(*this) = (*this) + buffer;
		return *this;
	}

	LEString& operator + (LEFloat value) {
		LEChar buffer[ms_BufferSize] = { '\0' };
		::sprintf(buffer, "%.5f", value);
		(*this) = (*this) + le_char_to_wchar(buffer).c_str();
		return *this;
	}

	LEString& operator + (LEWChar value) {
		LEWChar buffer[1] = { L'\0' };
		::wsprintf(buffer, L"%c", value);
		(*this) = (*this) + buffer;
		return *this;
	}

	LEString& operator + (LEWChar* value) {
		uint32_t length = ::wcslen(value);
		if (length + this->m_length >= this->m_capacity - 1) {
			this->setCapacity((length + this->m_length + 1) * LE_COLLECTION_EXPAND_FACTOR);
		}
		::wcscat(this->m_pData, value);
		this->m_length += length;
		return *this;
	}

	void operator += (int value) {
		this->operator+(value);
	}

	void operator += (LEFloat value) {
		this->operator+(value);
	}

	void operator += (LEWChar value) {
		this->operator+(value);
	}

	void operator += (LEWChar* value) {
		this->operator+(value);
	}

	uint32_t findFirst(const LEWChar* val) {
		return le_kmp_match(this->m_pData, val);
	}

	uint32_t findLast(const LEWChar* val) {
		return le_kmp_match_reversed(this->m_pData, val);
	}

	LEString substr(uint32_t startPos, uint32_t size) {
		if (startPos < 0 || startPos >= m_length) {
			return LEString();
		}
		else {
			LEString ret;
			ret.m_pData = new LEWChar[size + 1];
			LEWChar* ptr = m_pData;
			for (int i = 0; i < startPos; i++) {
				ptr++;
			}
			::wcscpy(ret.m_pData, ptr);
			return ret;
		}

	}

	LEString* split(LEWChar ch) {

	}

	LEString* split(std::initializer_list<LEWChar> chars) {

	}

	void replace(LEWChar from, const LEWChar to) {
		for (uint32_t i = 0; i < this->m_length; i++) {
			if (this->m_pData[i] == from) {
				this->m_pData[i] = to;
			}
		}
	}

	void toUpper() {
		for (uint32_t i = 0; i < this->m_length; i++) {
			if (this->m_pData[i] >= L'a' && this->m_pData[i] <= L'z') {
				this->m_pData[i] = L'A' + (this->m_pData[i] - L'a');
			}
		}
	}

	void toLower() {
		for (uint32_t i = 0; i < this->m_length; i++) {
			if (this->m_pData[i] >= L'A' && this->m_pData[i] <= L'Z') {
				this->m_pData[i] = L'a' + (this->m_pData[i] - L'A');
			}
		}
	}

	std::string toStdString() {
		return le_wchar_to_char(this->m_pData);
	}

	std::wstring toStdWString() {

	}

	friend bool operator == (const LEString& s0,
							 const LEString& s1) {
		return s0.m_length == s1.m_length && 
			   ::wcscmp(s0.m_pData, s1.m_pData) == 0;
	}

	// Type cast
	const LEWChar* constStr() const{
		return m_pData;
	}

	static LEString sprintf(LEWChar* fmt , ...) {

	}

	static LEString fromStdString(const std::string &str) {
		std::wstring wstr = le_char_to_wchar(str);
		return LEString(wstr.c_str());
	}

	static LEString fromStdWString(const std::wstring& wstr) {
		return LEString(wstr.c_str());
	}
};

LE_NAMESPACE_END

template<>
struct Lampyris::LEHash<Lampyris::LEString> {
	LE_FORCEINLINE uint32_t operator()(const Lampyris::LEString& value) {
		const LEWChar* cStr = value.constStr();
		return std::hash<std::wstring>()(cStr);
	}
};

#endif // !LE_STRING_H



