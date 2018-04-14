#ifndef _KARIN_TEXTBROWSER_H
#define _KARIN_TEXTBROWSER_H

#include <QTextBrowser>

namespace karin
{
	class chm_engine;

	class text_browser : public QTextBrowser
	{
		Q_OBJECT
		public:
			text_browser(QWidget *widget = 0);
			virtual ~text_browser();
			void keyPressEvent(QKeyEvent *event);
			int getVScrollBarPosition() const;
			void setVScrollBarPosition( int pos );
			int getHScrollBarPosition() const;
			void setHScrollBarPosition( int pos );
			void openUrl(const QString &url);
			void setChmEngine(chm_engine *e);
			void init();
			bool slideMode() const;
			
			public Q_SLOTS:
				virtual void home();
			virtual void setSource(const QUrl &url);
			void setSlideMode(bool b);

Q_SIGNALS:
			void scrollHBarValueChanged(int value);
			void scrollVBarValueChanged(int value);
			void scrollHBarRangeChanged(int min, int max);
			void scrollVBarRangeChanged(int min, int max);
			void slideModeChanged(bool b);

		protected:
			virtual QVariant loadResource(int type, const QUrl &name);
			virtual void mouseMoveEvent(QMouseEvent *e);
			virtual void mousePressEvent(QMouseEvent *e);
			virtual void mouseReleaseEvent(QMouseEvent *e);
			virtual void mouseDoubleClickEvent(QMouseEvent *e);

		private:
			chm_engine *engine;
			bool slide_mode;
			int start_x;
			int start_y;
			int last_x;
			int last_y;
			bool mouse_pressed;
			bool zoom_dir;

		private:
			text_browser(const text_browser &other);
			text_browser & operator= (const text_browser &other);
	};
}

#endif
