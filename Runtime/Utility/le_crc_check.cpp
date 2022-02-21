/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Utility
 * \File:    le_crc_check.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_crc_check.h"
#include <Runtime/Base/le_safe_release.h>

uint32_t LECRCCheck::calculate(const std::string& fname) {
	// get the size of given file
	FILE* fp;
	if ((fp = ::fopen(fname.c_str(), "r")) == NULL) {
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	uint32_t size = ftell(fp);
	// allocate temp buffer to store bytes in file.
	char* bytes = new char[size];
	// read bytes in file.
	fseek(fp, 0, SEEK_SET);
	fread(bytes, size, 1, fp);
	fclose(fp);
	// CRC calculation
	uint32_t crcTable[256], crcTmp1;
	for (int i = 0; i < 256; i++) {
		crcTmp1 = i;
		for (int j = 8; j > 0; j--) {
			if (crcTmp1 & 1) crcTmp1 = (crcTmp1 >> 1) ^ 0xEDB88320L;
			else crcTmp1 >>= 1;
		}
		crcTable[i] = crcTmp1;
	}
	uint32_t crcTmp2 = 0xFFFFFFFF;
	while (size--) {
		crcTmp2 = ((crcTmp2 >> 8) & 0x00FFFFFF) ^ crcTable[(crcTmp2 ^ (*bytes)) & 0xFF];
		bytes++;
	}
	LE_SAFE_RELEASE_ARRAY(bytes);
	return (crcTmp2 ^ 0xFFFFFFFF);
}
