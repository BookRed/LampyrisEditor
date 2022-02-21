#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_asset_importer.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
// STL Includes
#include <string>

template<typename T>
class LEAssetImporter {
protected:
	std::string m_path;
public:
	            LEAssetImporter(const std::string& path):m_path(path) {}

	virtual    ~LEAssetImporter() = default;

	virtual T*  registerAsset() const = 0 ;
};