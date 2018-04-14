#ifndef _KARIN_PAGEVIEW_H
#define _KARIN_PAGEVIEW_H

#include <MContainer>

class MAction;
class MSlider;
class MLabel;
class MLinearLayoutPolicy;

namespace karin
{
	class chm_engine;
	class text_browser;
	class chm_viewer;
	class tool_bar;
	class bookmark_dialog;

	class page_view : public MContainer
	{
		Q_OBJECT

		public:
			page_view(QGraphicsItem *parent = 0);
			virtual ~page_view();
			void setChmEngine(chm_engine *e);

			public Q_SLOTS:
				void openUrl(const QString &url = QString());
			void init(bool success);
			void checkFavorite();

			private Q_SLOTS:
				void zoomIn();
			void zoomOut();
			void scrollHbar(int value);
			void scrollVbar(int value);
			void slideVBar(int value);
			void slideHBar(int value);
			void setLabelIsBuddy(const QUrl &url);
			void chooseEncode(const QString &e);
			void openEncodeDialog();
			void openBookmarkDialog();
			void setSlideSelectMode();

Q_SIGNALS:
			void addFavorite();

		private:
			enum view_mode
			{
				slide_mode = 0,
				select_mode
			};

		private:
			view_mode slide_or_select_mode;
			chm_engine *engine;
			text_browser *textbrowser;
			MLinearLayoutPolicy *mainpolicy;
			MLinearLayoutPolicy *subpolicy;
			MLabel *label;
			MSlider *vslider;
			MSlider *hslider;
			tool_bar *toolbar;
			MAction *zoomin;
			MAction *zoomout;
			MAction *home;
			MAction *prev;
			MAction *next;
			MAction *reload;
			MAction *codec;
			MAction *mark;
			MAction *add;
			MAction *slider;
			MAction *copy;
			bookmark_dialog *bookmark;

		private:
			page_view(const page_view &other);
			page_view & operator= (const page_view &other);

	};
}

#endif
