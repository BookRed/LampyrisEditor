#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_obj_importer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_asset_importer.h"
#include "Runtime/Resource/le_texture.h"
// QT Includes
#include <qimage.h>
class LEImageImporter:public LEAssetImporter<LETexture2D> {
private:
	static uint32_t        getMinimumUpperPow2Number(uint32_t value);
	static constexpr int   ms_pow2Numbers[] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192 };

	unsigned char*         m_pData{ nullptr };
	uint32_t               m_width;
	uint32_t               m_height;
	QImage                 m_image;

public:
	using LEAssetImporter::LEAssetImporter;

	void                   loadImageData();

	LETexture2D*           registerAsset() const;
};
