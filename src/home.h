#ifndef _KARIN_HOME_H
#define _KARIN_HOME_H

#include <MApplicationPage>

namespace karin
{
	class header_view;
	class home : public MApplicationPage
	{
		Q_OBJECT

		public:
			home(QGraphicsItem *parent = 0);
			virtual ~home();

Q_SIGNALS:
			void openFileChooser();
			void openHistoryList();
			void openFavoriteList();
			void openSettingControl();

			private Q_SLOTS:
			void openAboutDialog();

		private:
			home(const home &other);
			home & operator= (const home &other);
	};
}

#endif
