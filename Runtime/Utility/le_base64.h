#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Utility
 * \File:    le_base64.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_BASE64
#define LE_BASE64

// STL Includes
#include <string>

class LEBase64 {
private:
	static constexpr unsigned int ms_maxBufferSize = 4096;
	static char                   ms_buffer[ms_maxBufferSize];
	constexpr static char ms_characters[] = {
		// 00 - 25
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
		'O','P','Q','R','S','T','U','V','W','X','Y','Z',
		// 26 - 51
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
		'o','p','q','r','s','t','u','v','w','x','y','z',
		// 52 - 64
		'0','1','2','3','4','5','6','7','8','9','+','/','='
	};

	static int locateCharacter(char p) {
		if (p >= 'A' && p <= 'Z')
			return (p - 'A');
		else if (p >= 'a' && p <= 'z')
			return (p - 'a' + 26);
		else if (p >= '0' && p <= '9')
			return (p - '0' + 26 + 26);
		else if (p == '+')
			return 62;
		else if (p == '/')
			return 63;
		else if (p == '=')
			return 64;
		return -1;
	}
public:
	static char* encode(const char* pData, int byteCount) {
		if (byteCount > ms_maxBufferSize) {
			// [Fatal Error]:
			return nullptr;
		}
		int nLeft = 3 - byteCount % 3;
		// the total length will become 4/3 times the original
        // the byte count of result string should be: length*4/3 + 1('\0')
		char* result = new char[((byteCount + nLeft) * 4 / 3 + 1)]{'\0'};
		char ch[4];
		int i, j;
		for (i = 0, j = 0; i < (byteCount - byteCount % 3); i += 3, j += 4) {
			ch[0] = (char)((pData[i] & 0xFC) >> 2);
			ch[1] = (char)((pData[i] & 0x03) << 4 | (pData[i + 1] & 0xF0) >> 4);
			ch[2] = (char)((pData[i + 1] & 0x0F) << 2 | (pData[i + 2] & 0xC0) >> 6);
			ch[3] = (char)((pData[i + 2] & 0x3F));
			result[j]     = ms_characters[ch[0]];
			result[j + 1] = ms_characters[ch[1]];
			result[j + 2] = ms_characters[ch[2]];
			result[j + 3] = ms_characters[ch[3]];
		}
		if (nLeft == 2) {
			ch[0] = (char)((pData[i] & 0xFC) >> 2);
			ch[1] = (char)((pData[i] & 0x3) << 4);
			ch[2] = 64;
			ch[3] = 64;
			result[j]     = ms_characters[ch[0]];
			result[j + 1] = ms_characters[ch[1]];
			result[j + 2] = ms_characters[ch[2]];
			result[j + 3] = ms_characters[ch[3]];
		}
		else if (nLeft == 1) {
			ch[0] = (char)((pData[i] & 0xFC) >> 2);
			ch[1] = (char)((pData[i] & 0x03) << 4 | (pData[i + 1] & 0xF0) >> 4);
			ch[2] = (char)((pData[i + 1] & 0x0F) << 2);
			ch[3] = 64;
			result[j]     = ms_characters[ch[0]];
			result[j + 1] = ms_characters[ch[1]];
			result[j + 2] = ms_characters[ch[2]];
			result[j + 3] = ms_characters[ch[3]];
		}
		return result;
	}
	static const char* decode(const char* buffer,int byteCount) {
		if (byteCount > ms_maxBufferSize) {
			// [Fatal Error]:byteCount can not exceed ms_maxBufferSize
			return nullptr;
		}
		if (byteCount % 4 != 0) {
			// [Fatal Error]:Not valid base64-encoded data
			return nullptr;
		}
		char* data = ms_buffer;
		char p;
		char ch[4];
		int i, j;
		for (i = 0, j = 0; i < byteCount; i += 4, j += 3) {
			for (int z = 0; z < 4; z++) {
				p = (char)locateCharacter(buffer[i + z]);
				if (p == -1) {
					return nullptr;
				}
				ch[z] = p;
			}
			data[j]     = (char)((ch[0] & 0x3F) << 2 | (ch[1] & 0x30) >> 4);
			data[j + 1] = (char)((ch[1] & 0x0F) << 4 | (ch[2] & 0x3C) >> 2);
			data[j + 2] = (char)((ch[2] & 0x03) << 6 | (ch[3] & 0x3F));
		}
		return data;
	}

};

#endif // !LE_BASE64