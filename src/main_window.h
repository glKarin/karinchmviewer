#ifndef _KARIN_MAINWINDOW_H
#define _KARIN_MAINWINDOW_H

#include <MApplicationWindow>

namespace karin
{
	class file_chooser;
	class chm_viewer;
	class home;
	class record_manager;
	class setting_control;

	class main_window : public MApplicationWindow
	{
		Q_OBJECT

		public:
			main_window(const QString &file = QString(), const QString &page = QString(), QWidget *parent = 0);
			virtual ~main_window();
			void doShow();

			public Q_SLOTS:
				void openChm(const QString &file);

			private Q_SLOTS:
				void openFileChooser();
			void openHistoryList();
			void openFavoriteList();
			void openSettingControl();
			void changeOrientation(int i);
			void setWindowState(bool b);

		private:
			file_chooser *filechooser;
			chm_viewer *chmviewer;
			home *homepage;
			record_manager *history;
			record_manager *favorite;
			setting_control *setting;

		private:
			main_window(const main_window &other);
			main_window & operator= (const main_window &other);
	};
}

#endif
