#include "editable_delegate.h"

#include <QGraphicsAnchorLayout>
#include <MContentItem>
#include <MButton>

karin::editable_delegate::editable_delegate(QGraphicsItem *parent)
	:MListItem(parent),
	contentItem(new MContentItem(MContentItem::TwoTextLabels)),
	button(new MButton("icon-m-toolbar-delete", ""))
{
	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout;
	setPreferredHeight(contentItem -> preferredHeight());
	setMinimumHeight(contentItem -> minimumHeight());
	setMaximumHeight(contentItem -> maximumHeight());
	button -> setMaximumWidth(button -> minimumWidth());
	button -> setPreferredWidth(button -> minimumWidth());
	button -> setViewType(MButton::iconType);
	layout -> addAnchor(contentItem, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout -> addAnchor(button, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout -> addAnchor(contentItem, Qt::AnchorRight, button, Qt::AnchorLeft);
	layout -> addAnchor(button, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	setLayout(layout);
	connect(button, SIGNAL(clicked(bool)), this, SLOT(thisLongTap(bool)));
	connect(contentItem, SIGNAL(clicked()), this, SIGNAL(clicked()));
	connect(contentItem, SIGNAL(longTapped(const QPointF &)), this, SIGNAL(longTapped(const QPointF &)));
}

karin::editable_delegate::~editable_delegate()
{
}

void karin::editable_delegate::thisLongTap(bool b)
{
	Q_UNUSED(b);
	MListItem::longTap(button -> scenePos());
}

void karin::editable_delegate::setTitle(const QString &title)
{
	contentItem -> setTitle(title);
}

void karin::editable_delegate::setSubtitle(const QString &subtitle)
{
	contentItem -> setSubtitle(subtitle);
}

