/*!
 * \Lampyris GameEngine C++ Source File
 * \Module:  Editor
 * \File:    le_file_system_model.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#include <QDateTime>
#include <QUrl>
#include "le_file_system_model.h"

QIcon FileIconProvider::icon(const QFileInfo& info) const {
	if (!info.isDir()) {
		QList<QByteArray> supportFormats = QImageReader::supportedImageFormats();
		QString i = info.suffix();
		//if (supportFormats.contains(info.suffix().toLatin1())) {
		if(info.suffix() == "png" || info.suffix() == "jpg") {
			QPixmap map(info.filePath());
			map = map.scaled(64,64);
			return QIcon(map);
		}
	}
	return QFileIconProvider::icon(info);
}
