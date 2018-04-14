#include "header_view.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MSeparator>

#define HEADER_HEIGHT 60

karin::header_view::header_view(karin::header_view::karin_ViewType type, QGraphicsItem *parent)
	:MWidgetController(parent),
	label(new MLabel)
{
	MSeparator *separator = new MSeparator;
	MLayout *layout = new MLayout;
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	label -> setAlignment(Qt::AlignCenter);
	label -> setColor(QColor("#9E1B29"));
	setPreferredHeight(HEADER_HEIGHT);
	setMaximumHeight(HEADER_HEIGHT);
	setMinimumHeight(HEADER_HEIGHT);
	if(type == karin::header_view::footerType)
	{
		policy -> addItem(separator);
		policy -> addItem(label);
	}
	else
	{
		policy -> addItem(label);
		policy -> addItem(separator);
	}	
	setLayout(layout);
}

karin::header_view::~header_view()
{
}

void karin::header_view::setTitle(const QString &title)
{
	label -> setText(title);
}

void karin::header_view::setTitleColor(const QString &code)
{
	label -> setColor(QColor(code));
}

void karin::header_view::setTitleFont(const QFont &font)
{
	label -> setFont(font);
}
