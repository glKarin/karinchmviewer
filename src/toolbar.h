#ifndef _KARIN_TOOLBAR_H
#define _KARIN_TOOLBAR_H

#include <MPannableViewport>
#include <QList>

class MToolBar;
class QAction;
class MLayout;
class MContainer;
class MLinearLayoutPolicy;

namespace karin
{
	class tool_bar : public MPannableViewport
	{
		public:
			tool_bar(QGraphicsItem *parent = 0);
			virtual ~tool_bar();
			void addAction(QAction *action);
			//void addActions(QList<QAction *> actions);

		private:
			unsigned int tools;
			QList<MToolBar *> tb;
			MContainer *container;
			MLayout *layout;
			MLinearLayoutPolicy *policy;

		private:
			tool_bar(const tool_bar &other);
			tool_bar & operator= (const tool_bar &other);
	};
}

#endif
