/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Resource
 * \File:    le_font.cpp
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

// LE Includes
#include "le_font.h"

// initialization for LEObjects
LE_INITIAL_IMPL_BEGIN(LEFont)
{
	LE_INITIAL_METHOD(&LEFont::loadDefaultResource);
	LE_TERMINATE_METHOD(&LEFont::destoryDefaultResource);
	LE_PROPERTY_REGISTER_METHOD(&LEFont::initProperty);
	LE_PROPERTY_DESTORY_METHOD(&LEFont::terminateProperty);
	LE_CREATE_INSTANCE_METHOD(&LEFont::createInstance);
}
LE_INITIAL_IMPL_END(LEFont)

LE_ADD_PROPERTY_BEGIN(LEFont)
LE_ADD_PROPERTY_END

// static properties
LEFont* LEFont::ms_defaultFont = nullptr;

// methods
LEFont::LEFont() {

}

LEFont::LEFont(const std::string& filePath):
	LEResource(filePath) {
	FT_Library       m_library;
	FT_Face          m_face;
	::FT_Init_FreeType(&m_library);
	if (!m_library) {
		// [Fatal Error]: init freeType library failed
	}
	::FT_New_Face(m_library, filePath.c_str(), 0, &m_face);
	this->m_fontName = m_face->family_name;
}

LEFont::~LEFont() {

}

LEFont::ResourceType LEFont::getType() const {
	return ResourceType::RT_FONT;
}

bool LEFont::loadDefaultResource() {
	return 1;
}

bool LEFont::destoryDefaultResource() {
	return 1;
}

const LEFont* LEFont::getDefault() {
	return ms_defaultFont;
}

LECharacterInfo* LEFont::getCharacterInfo(wchar_t ch, int fontSize, bool bold, bool italic) {
	// get the index of the array according to given font style.
	int index = (int)(bold)+((int)(italic) << 1);
	// whether the fontSize used before ?
	if (this->m_fontTextureMap.contains(fontSize)) {
		LEFontTexture* fontTex = this->m_fontTextureMap[fontSize][index];
		if (fontTex != nullptr) {
			return fontTex->getCharacterInfo(ch);
		}
	}
	LEFontTexture* fontTex = new LEFontTexture(this->m_resourcePath, fontSize, bold, italic);
	this->m_fontTextureMap[fontSize][index] = fontTex;
	return fontTex->getCharacterInfo(ch);
}

void LEFont::saveFontTexture(int fontSize, bool bold, bool italic) {
	// get the index of the array according to given font style.
	int index = (int)(bold)+((int)(italic) << 1);
	// whether the fontSize used before ?
	if (this->m_fontTextureMap.contains(fontSize)) {
		LEFontTexture* fontTex = this->m_fontTextureMap[fontSize][index];
		fontTex->saveAsPNG();
	}
}

void LEFont::onLoaded() {
}

void LEFont::onUnloaded() {
}

LEFontTexture::LEFontTexture():
	m_isValid(false) {}

LEFontTexture::LEFontTexture(const std::string& fontFile, int fontSize, bool bold, bool italic) :
	m_fontSize(fontSize), m_bold(bold), m_italic(italic) {
	::FT_Init_FreeType(&this->m_library);
	if (!this->m_library) {
		// [Fatal Error]: init freeType library failed
	}
	::FT_New_Face(this->m_library, fontFile.c_str(), 0, &this->m_face);
	::FT_Set_Char_Size(this->m_face, fontSize << 6, fontSize << 6, 72, 72);
	this->m_face->style_flags |= (bold ? FT_STYLE_FLAG_BOLD : 0);
	this->m_face->style_flags |= (italic ? FT_STYLE_FLAG_ITALIC : 0);
	if (italic) {
		FT_Matrix m;
		m.xx = 0x10000L;
		m.xy = 0.5f * 0x10000L;
		m.yx = 0;
		m.yy = 0x10000L;
		FT_Set_Transform(this->m_face, &m, NULL);
	}
	this->m_fontName = m_face->family_name;
}

LEFontTexture::~LEFontTexture() {
	::FT_Done_Face(this->m_face);
	::FT_Done_FreeType(this->m_library);
}

LECharacterInfo* LEFontTexture::getCharacterInfo(wchar_t ch) {
	if (this->m_charMap.contains(ch)) {
		return &this->m_charMap[ch];
	}
	FT_Error error = ::FT_Load_Glyph(this->m_face,
		::FT_Get_Char_Index(this->m_face, ch),
		FT_LOAD_DEFAULT);
	// freeType error: load glyph failed
	if (error != 0) {
		return nullptr;
	}
	FT_Glyph glyph;
	error = ::FT_Get_Glyph(this->m_face->glyph, &glyph);
	// freeType error: get glyph failed
	if (error != 0) {
		return nullptr;
	}
	::FT_Render_Glyph(this->m_face->glyph, FT_RENDER_MODE_MONO);
	error = ::FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	// freeType error: render glyph to bitmap failed
	if (error != 0) {
		return nullptr;
	}

	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;
	if (this->m_bold) {
		::FT_Bitmap_Embolden(this->m_library, &bitmap, 60, 0);
	}
	int width = bitmap.width;
	int height = bitmap.rows;
	int dataWidth = width + (4 - width % 4);
	int rgbaSize = (this->m_fontSize * 2) * this->m_fontSize * 4;
	unsigned char* data = new unsigned char[rgbaSize] { 0 };

	le_convert_gray_to_rgba(&bitmap, data);

	LECharacterInfo& charInfo = this->m_charMap[ch];
	// whether we should write rendered character to the next line of the texture
	if (this->m_xStart + this->m_fontSize > ms_textureSize) {
		this->m_xStart = 0;
		this->m_yStart += this->m_fontSize;
	}
	charInfo.x0 = this->m_xStart;
	charInfo.y0 = this->m_yStart;
	if (bitmap.width == 0 || bitmap.rows == 0) {
		this->m_xStart += (this->m_fontSize / 2);
		charInfo.x1 = this->m_xStart + bitmap.width + 0.8 * this->m_fontSize;
		charInfo.y1 = this->m_yStart + bitmap.rows;
		charInfo.offsetY = 0;
		charInfo.offsetX = 0;
	}
	else {
		// replace pixel data
		for (int i = 0; i < bitmap.rows; i++) {
			for (int j = 0; j < bitmap.width; j++) {
				unsigned int* pCharDataDst = ((unsigned int*)this->m_data + (ms_textureSize * (i + this->m_yStart) + this->m_xStart + j));
				unsigned int* pCharDataSrc = ((unsigned int*)data + (bitmap.width * i + j));
				*pCharDataDst = *pCharDataSrc;
			}
		}
		charInfo.x1 = this->m_xStart + bitmap.width;
		charInfo.y1 = this->m_yStart + bitmap.rows;
		charInfo.offsetX = bitmap_glyph->left;
		charInfo.offsetY = bitmap_glyph->top;
		this->m_xStart += (bitmap.width + 5);
	}
	LE_SAFE_RELEASE_ARRAY(data);
	return &charInfo;
}

void LEFontTexture::saveAsPNG() {
	// format: fontName_fontSize_xy.png
	// where x,y represent the style of font (bold, italic), with value zero or one
	std::string fileName = (std::stringstream()
		<< this->m_fontName << "_"
		<< this->m_fontSize << "_"
		<< (int)this->m_bold << (int)this->m_italic
		<< ".png").str();
	LEPngBuilder::createFromRGBA(fileName, m_data, ms_textureSize, ms_textureSize);
}

bool LEFontTexture::isValid() const {
	return this->m_isValid;
}
