/*!
 * \Lampyris GameEngine C++ Inline File
 * \Module:  Common
 * \File:    le_hash.inl
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#include "le_hash.h"

LEUInt LEHash<LEString>::operator ()() {
	LEUInt operator()(const LEString& value) {
		unsigned int seed = 131;
		unsigned int hash = 0;
		const LEWChar* wcstr = value.constStr();
		while (*wcstr) {
			hash = hash * seed + (*wcstr++);
		}
		return (hash & 0x7FFFFFFF);
	}
};

