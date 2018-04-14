#ifndef _KARIN_CHMVIEWER_H
#define _KARIN_CHMVIEWER_H

#include <MApplicationPage>
#include <MAction>

class MContainer;

namespace karin
{
	class chm_engine;
	class page_view;
	class directory_treeview;
	class index_listview;
	class search_listview;
	class chm_viewer;

	class tab_action : public MAction
	{
		Q_OBJECT

		public:
			tab_action(QObject *parent = 0);
			tab_action(const QString &title, QObject *parent = 0);
			tab_action(const QString &iconId, const QString &title, QObject *parent = 0);
			virtual ~tab_action();
			void setTabWidget(QGraphicsWidget *widget, const QString &title = QString());

Q_SIGNALS:
			void showTabWidget(QGraphicsWidget *widget, const QString &title);

		private Q_SLOTS:
			void doTrigger(bool checked);

		private:
			QGraphicsWidget *tab_widget;
			QString tab_title;

			friend class chm_viewer;

		private:
			tab_action(const tab_action &other);
			tab_action & operator= (const tab_action &other);
	};

	class chm_viewer : public MApplicationPage
	{
		Q_OBJECT

		public:
			chm_viewer(QGraphicsItem *parent = 0);
			virtual ~chm_viewer();
			bool openChm(const QString &file, const QString &p = QString());

			private Q_SLOTS:
				void changeCentralWidget(QGraphicsWidget *widget, const QString &title);
				void changeCentralWidget(QAction *action);
			void openUrl(const QString &url);
			void addFavorite();
			void showInfoBanner(int t, const QString &info);

		private:
			chm_engine *engine;
			MContainer *central_widget;
			tab_action *pageView;
			tab_action *dirlistView;
			tab_action *indexlistView;
			tab_action *searchView;
			page_view *page;
			directory_treeview *directory;
			index_listview *index;
			search_listview *search;

		private:
			chm_viewer(const chm_viewer &other);
			chm_viewer & operator= (const chm_viewer &other);
	};
}

#endif
