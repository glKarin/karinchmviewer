#include "toolbar.h"

#include <MToolBar>
#include <MContainer>
#include <MLinearLayoutPolicy>
#include <MLayout>

karin::tool_bar::tool_bar(QGraphicsItem *parent)
	:MPannableViewport(parent),
	tools(0),
	container(new MContainer),
	layout(new MLayout),
	policy(new MLinearLayoutPolicy(layout, Qt::Horizontal))
{
	container -> setHeaderVisible(false);
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	container -> centralWidget() -> setLayout(layout);
	setWidget(container);
	setPreferredHeight(60);
	setMinimumHeight(60);
	setMaximumHeight(60);
	setPanDirection(Qt::Horizontal);
}

karin::tool_bar::~tool_bar()
{
	//while(tb.size())
		//delete tb.takeAt(0);
}

void karin::tool_bar::addAction(QAction *action)
{
	if(tools % 5 == 0)
	{
		MToolBar *bar = new MToolBar;
		tb.push_back(bar);
		policy -> addItem(bar);
	}
	tools++;
	MToolBar *t = tb.last();
	t -> addAction(action);
	//container -> setTitle(tr("ToolBar Items") + ": " + QString::number(tools));
}
