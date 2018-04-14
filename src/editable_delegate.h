#ifndef _KARIN_EDITABLEDELEGATE_H
#define _KARIN_EDITABLEDELEGATE_H

#include <MListItem>

class MContentItem;
class MButton;

namespace karin
{
	class editable_delegate : public MListItem
	{
		Q_OBJECT

		public:
			editable_delegate(QGraphicsItem *parent = 0);
			virtual ~editable_delegate();
			void setTitle(const QString &title);
			void setSubtitle(const QString &subtitle);

			private Q_SLOTS:
				void thisLongTap(bool b);

		private:
			MContentItem *contentItem;
			MButton *button;

		private:
			editable_delegate(const editable_delegate &other);
			editable_delegate & operator= (const editable_delegate &other);

	};
}

#endif
