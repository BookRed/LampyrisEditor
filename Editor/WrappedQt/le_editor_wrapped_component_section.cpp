#include "le_editor_wrapped_component_section.h"

QPixmap* LEEditorWrappedComponentSection::ms_iconExpand;
QPixmap* LEEditorWrappedComponentSection::ms_iconCollapse;

void LEEditorWrappedComponentSection::createLayout() {
	// main layout
	this->setLayout(this->m_layout);
	this->m_layout->setContentsMargins(0, 0, 0, 0);
	this->m_layout->addWidget(this->m_container);
	this->m_container->setLayout(this->m_containerlayout);
	// container layout
	this->m_containerlayout->setContentsMargins(0, 2, 0, 2);
	this->m_containerlayout->setSpacing(3);
	this->m_containerlayout->addWidget(this->m_iconLabel);
	if (this->m_checkBox) {
		this->m_containerlayout->addWidget(this->m_checkBox);
	}
	this->m_checkBox->setFixedHeight(20);
	this->m_containerlayout->addWidget(this->m_componentNameLabel);
	this->m_containerlayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding));
	this->m_iconLabel->setPixmap(*ms_iconExpand);
}

void LEEditorWrappedComponentSection::connectSignal() {
	QObject::connect(this->m_checkBox, &QCheckBox::toggled, [=]() {
		// set the enabled state of component
	});
}

void LEEditorWrappedComponentSection::mousePressEvent(QMouseEvent* event) {
	this->m_isOpened = !this->m_isOpened;
	if(this->m_isOpened) {
		this->m_iconLabel->setPixmap(*ms_iconExpand);
	} else {
		this->m_iconLabel->setPixmap(*ms_iconCollapse);
	}
	emit signalEventDispatch(this);
	QWidget::mousePressEvent(event);
}

LEEditorWrappedComponentSection::LEEditorWrappedComponentSection(const QString& componentName, bool canSetEnabled):
	m_container         (new QWidget(this)),
	m_isOpened          (true),
	m_layout            (new QHBoxLayout(this)),
	m_containerlayout   (new QHBoxLayout(this)),
	m_checkBox          (canSetEnabled ? new QCheckBox(this):nullptr),
	m_componentNameLabel(new QLabel(componentName, this)),
	m_iconLabel         (new QLabel(this)) {
	// init default resources
	if (!ms_iconCollapse) {
		ms_iconExpand = new QPixmap(QPixmap("res\\property_editor\\icon_expand.png").scaled(15,15));
		ms_iconCollapse = new QPixmap(QPixmap("res\\property_editor\\icon_collapse.png").scaled(15, 15));
	}
	this->createLayout();
	this->connectSignal();
	if(!canSetEnabled) {
		this->m_checkBox->setChecked(true);
		this->m_checkBox->setCheckable(false);
	}
}

LEEditorWrappedComponentSection::~LEEditorWrappedComponentSection() {
		
}

bool LEEditorWrappedComponentSection::isOpened() const {
	return this->m_isOpened;
}
