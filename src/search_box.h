#ifndef _KARIN_SEARCHBOX_H
#define _KARIN_SEARCHBOX_H

#include <MToolBar>

class MAction;
class MTextEdit;

namespace karin
{
	class search_box : public MToolBar
	{
		Q_OBJECT

		public:
			search_box(QGraphicsItem *parent = 0);
			virtual ~search_box();
			QString text() const;
			void setPrompt(const QString &s);
			void setToolVisible(bool b);

Q_SIGNALS:
			void textChanged();
			void returnPressed();
			void triggered(bool checked = false);

			public Q_SLOTS:
				void setEnabled(bool b);
			void setCheckable(bool b);

		private:
			MTextEdit *edit;
			MAction *search;

		private:
			search_box(const search_box &other);
			search_box & operator= (const search_box &other);
	};
}

#endif
