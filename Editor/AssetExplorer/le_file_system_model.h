#pragma once
/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_file_system_model.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_FILE_SYSTEM_MODEL_H
#define LE_FILE_SYSTEM_MODEL_H

#include "../../Common/le_global.h"

#include <QImageReader>
#include <QFileSystemModel>
#include <QFileIconProvider>

class FileIconProvider : public QFileIconProvider
{
public:
	virtual QIcon icon(const QFileInfo& info) const;
};
class LEEditorFileSystemModel:public QFileSystemModel
{
private:

public:
	LEEditorFileSystemModel(QObject* parent = nullptr) {
		this->setIconProvider(new FileIconProvider());
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role = 2) const {
		switch (role) {
		case Qt::DecorationRole:
			if (section == 0) {
				QImage pixmap(16, 1, QImage::Format_Mono);
				pixmap.fill(0);
				pixmap.setAlphaChannel(pixmap.createAlphaMask());
				return pixmap;
			}
			break;
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
		}

		if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
			return QAbstractItemModel::headerData(section, orientation, role);

		QString returnValue;
		switch (section) {
		case 0: returnValue = tr("Asset Folder"); break;
		default: return QVariant();
		}
		return returnValue;
	}
	
};

#endif