/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_safe_release.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SAFE_RELEASE_H
#define LE_SAFE_RELEASE_H

#define LE_SAFE_RELEASE(p) if(p) {delete p;}
#define LE_SAFE_RELEASE_ARRAY(p) if(p) {delete[] p;}

#endif // !LE_SAFE_RELEASE_H
