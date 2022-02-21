/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_obj_importer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// LE Includes
#include "le_image_importer.h"

uint32_t LEImageImporter::getMinimumUpperPow2Number(uint32_t value) {
	if (std::_Is_pow_2(value)) {
		return value;
	}
	for (int i = 0; i < sizeof(ms_pow2Numbers) / sizeof(int); i++) {
		if (value <= ms_pow2Numbers[i]) {
			return ms_pow2Numbers[i];
		}
	}
	return 0;
}

void LEImageImporter::loadImageData() {
	this->m_image = QImage(QString::fromStdString(this->m_path));
	if(this->m_image.format()!= QImage::Format_RGBA8888) {
		this->m_image = this->m_image.convertToFormat(QImage::Format_RGBA8888);
	}
	if(this->m_image.isNull()) {
		return;
	}
	// scale size
	this->m_width = this->m_image.width();
	this->m_height = this->m_image.height();

	const uint32_t widthPow2 = this->getMinimumUpperPow2Number(this->m_width);
	const uint32_t heightPow2 = this->getMinimumUpperPow2Number(this->m_height);

	// need to resize
	if (this->m_width != widthPow2 || this->m_height != heightPow2) {
		this->m_image = this->m_image.scaled(widthPow2, heightPow2);
		this->m_width = widthPow2;
		this->m_height = heightPow2;
	}
	this->m_pData = this->m_image.bits();
}

LETexture2D* LEImageImporter::registerAsset() const {
	if(!this->m_width || !this->m_height || !this->m_pData) {
		return nullptr;
	}
	LETexture2D* pTexture = new LETexture2D(this->m_width,this->m_height,this->m_pData);
	return pTexture;
}
