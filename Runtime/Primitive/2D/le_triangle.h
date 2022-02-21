#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Common
 * \File:    le_triangle.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// equilateral triangle
class LETriangleData {
	static float* getVertexData() {
		static float data[]{
			// top
			0.0f, 1.0f, 0.0f,
			// left-bottom
			-0.7071067f,0.0f,0.0f,
			// right-bottom
			0.7071067f,0.0f,0.0f
		};
		return data;
	}
};

class LERectangleData {
	static 
};