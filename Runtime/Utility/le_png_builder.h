/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_png_builder.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// References: 
// [0]: How to use libpng to create PNG based on given pixel data
// https://blog.csdn.net/zengzhihao/article/details/107705792

#ifndef LE_PNG_BUILDER_H
#define LE_PNG_BUILDER_H

// LE Includes
#include "../Base/le_global.h"

// PNG Includes
#include <png/png.h>

enum PNGResult{
	SUCCESS,
	FAILED_CREATE_FILE,
	FAILED_CRTETE_WRITE_STRUCT,
	FAILED_CRTETE_INFO_STRUCT,
	FAILED_CALL_JUMBUF
};

class LEPngBuilder {
private:
	static PNGResult create(const std::string& fileName,
							unsigned char*     pixel, 
							int                width, 
							int                height,
							int                mode) {
		png_structp png;
		png_infop   pngInfo;

		int rowWidth;
		int bitDepth = 1;

		if (mode == PNG_COLOR_TYPE_GRAY) {
			bitDepth = 1;
			rowWidth = width;
		}
		else if (mode == PNG_COLOR_TYPE_RGB) {
			bitDepth = 8;
			rowWidth = width * 3;
		}
		else if (mode == PNG_COLOR_TYPE_RGBA) {
			bitDepth = 8;
			rowWidth = width * 4;
		}
		else {
			bitDepth = 0;
			rowWidth = 0;
		}
		FILE* file = fopen(fileName.c_str(), "wb");
		if (file == NULL) {
			return PNGResult::FAILED_CREATE_FILE;
		}

		png = ::png_create_write_struct(PNG_LIBPNG_VER_STRING, 
									    NULL, 
									    NULL, 
									    NULL);
		if (png == NULL) {
			fclose(file);
			return PNGResult::FAILED_CRTETE_WRITE_STRUCT;
		}

		pngInfo = ::png_create_info_struct(png);
		if(!png) {
			fclose(file);
			return PNGResult::FAILED_CRTETE_INFO_STRUCT;
		}
		if (setjmp(png_jmpbuf(png))) {
			fclose(file);
			return PNGResult::FAILED_CALL_JUMBUF;
		}
		::png_init_io(png, file);
		::png_set_IHDR(png,
					   pngInfo,
					   width, height,
					   bitDepth,                    // bit_depth
					   mode,			            // color_type
					   PNG_INTERLACE_NONE,		    // interlace or not
					   PNG_COMPRESSION_TYPE_DEFAULT,// compression
					   PNG_FILTER_TYPE_DEFAULT);    // filter
		::png_set_packing(png);					 
		::png_write_info(png, pngInfo);

		// a pointer to record the address of the next row's pixel data
		unsigned char* pPixel = pixel;
		// write pixel data to png 
		for (int currentRow = 0; currentRow < height; currentRow++) {
			::png_write_row(png, pPixel);
			pPixel += rowWidth; 
		}
		::png_write_end(png, pngInfo);
		::png_destroy_write_struct(&png, &pngInfo);
		fclose(file);
		return PNGResult::SUCCESS;
	}
public:
	static bool createFromRGBA(const std::string& fileName,
							   unsigned char*     pixel, 
							   int				  width,
							   int				  height) {
		if (create(fileName, pixel,
				   width, height, 
				   PNG_COLOR_TYPE_RGBA) 
				   == PNGResult::SUCCESS) {
			return true;
		} 
		else {
			return false;
		}
	}

	static bool createFromRGB(const std::string& fileName,
							  unsigned char*     pixel,
							  int                width,
							  int                height) {
		if (create(fileName, pixel,
				   width, height,
				   PNG_COLOR_TYPE_RGB)
				   == PNGResult::SUCCESS) {
			return true;
		}
		else {
			return false;
		}
	}

	static bool createFromGray(const std::string& fileName,
							   unsigned char*     pixel,
							   int                width,
							   int                height) {
		if (create(fileName, pixel,
				   width, height,
				   PNG_COLOR_TYPE_GRAY) == PNGResult::SUCCESS) {
			return true;
		}
		else {
			return false;
		}
	}
};

#endif // !LE_PNG_BUILDER_H