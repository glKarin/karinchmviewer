#include "search_box.h"

#include <MAction>
#include <MWidgetAction>
#include <MTextEdit>

karin::search_box::search_box(QGraphicsItem *parent)
	:MToolBar(parent),
	edit(new MTextEdit),
	search(new MAction("icon-m-toolbar-search", tr("Search"), this))
{
	MWidgetAction *editAction = new MWidgetAction(this);
	edit -> setInputMethodPredictionEnabled(false);
	edit -> setInputMethodAutoCapitalizationEnabled(false);
	edit -> setInputMethodCorrectionEnabled(false);
	search -> setLocation(MAction::ToolBarLocation);
	editAction -> setLocation(MAction::ToolBarLocation);
	editAction -> setWidget(edit);
	addAction(editAction);
	addAction(search);
	connect(edit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
	connect(edit, SIGNAL(returnPressed()), this, SIGNAL(returnPressed()));
	connect(search, SIGNAL(triggered(bool)), this, SIGNAL(triggered(bool)));
}

karin::search_box::~search_box()
{
}

void karin::search_box::setEnabled(bool b)
{
	edit -> clear();
	edit -> setReadOnly(!b);
	search -> setEnabled(b);
}

void karin::search_box::setCheckable(bool b)
{
	search -> setEnabled(b);
}

QString karin::search_box::text() const
{
	return edit -> text();
}

void karin::search_box::setPrompt(const QString &s)
{
	edit -> setPrompt(s);
}

void karin::search_box::setToolVisible(bool b)
{
	search -> setVisible(b);
}
