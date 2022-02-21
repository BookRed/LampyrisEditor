#include "le_launcher_tab_style.h"
#include "le_launcher_tab_widget.h"

QSize LELauncherTabStyle::sizeFromContents(ContentsType type,
	const QStyleOption* option,
	const QSize& size,
	const QWidget* widget) const {
	QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
	if (type == QStyle::CT_TabBarTab) {
		s.transpose();
		s.rwidth() = 120;
		s.rheight() = 45;
	}
	return s;
}

void LELauncherTabStyle::drawControl(ControlElement element, 
	const QStyleOption* option, 
	QPainter* painter, 
	const QWidget* widget) const {
	if (element == CE_TabBarTabLabel) {
		if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
			QRect allRect = tab->rect;

			if (tab->state & QStyle::State_Selected) {
				painter->save();
				painter->setPen(0xffb342);
				painter->setBrush(QBrush(0xffb342));
				painter->drawRect(allRect.adjusted(6, 6, -6, -6));
			}
			QTextOption option;
			option.setAlignment(Qt::AlignCenter);
			painter->setPen(0xffffff);
			painter->drawText(allRect, tab->text, option);
			painter->restore();

			return;
		}
	}
	if (element == CE_TabBarTab) {
		QProxyStyle::drawControl(element, option, painter, widget);
	}
}