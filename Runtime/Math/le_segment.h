#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Math
 * \File:    le_segment.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SEGNMENT_H
#define LE_SEGNMENT_H

// LE Includes
#include "le_ray.h"

class LESegment:public LERay {
private:
	glm::vec3 m_end_point;
	float     m_length;
public:
	           LESegment();
	           
	           LESegment(const glm::vec3& dir, 
	           	         const glm::vec3& origin, 
	           	         float length);
	           
	           LESegment(const glm::vec3& origin, 
	           	         const glm::vec3& end);
	           
	           LESegment(const LESegment& other);
	          
	LESegment& operator = (const LESegment& other);

	float      getLength() const;

	glm::vec3  getEndPoint() const;
};

#endif // !LE_SEGNMENT_H