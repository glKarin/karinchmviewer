#include "text_browser.h"
#include "chm_engine.h"
#include "karin_std.h"
#include "karin_ut.h"

#include <QVariant>
#include <QImage>
#include <QByteArray>
#include <QUrl>
#include <QKeyEvent>
#include <QScrollBar>
#include <MApplication>
#include <QDesktopServices>
#include <QProcess>
#ifdef _SIMLATOR
#include <QDebug>
#endif

karin::text_browser::text_browser(QWidget *parent)
	:QTextBrowser(parent),
	engine(0),
	slide_mode(true),
	start_x(-1),
	start_y(-1),
	last_x(-1),
	last_y(-1),
	mouse_pressed(false),
	zoom_dir(true)
{
	connect(QTextBrowser::verticalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollVBarValueChanged(int)));
	connect(QTextBrowser::horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollHBarValueChanged(int)));
	connect(QTextBrowser::verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(scrollVBarRangeChanged(int, int)));
	connect(QTextBrowser::horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(scrollHBarRangeChanged(int, int)));
  setWordWrapMode(QTextOption::WordWrap);
}

karin::text_browser::~text_browser()
{
	engine = 0;
}

void karin::text_browser::setChmEngine(chm_engine *e)
{
	engine = e;
}

void karin::text_browser::keyPressEvent(QKeyEvent* event)
{
  int key = event -> key();
  Qt::KeyboardModifiers mods = event -> modifiers();
  if (mods & (Qt::ControlModifier | Qt::ShiftModifier ))
  {
    switch ( key ) 
    {
      case Qt::Key_Left:
      case Qt::Key_B:
				backward();
				break;
      case Qt::Key_Right:
      case Qt::Key_F:
				forward();
				break;
      case Qt::Key_Up:
      case Qt::Key_I:
				QTextBrowser::zoomIn(karin::ut::Instance() -> getSetting<int>(ZOOM_RANGE));
				break;
      case Qt::Key_Down:
      case Qt::Key_O:
				QTextBrowser::zoomOut(karin::ut::Instance() -> getSetting<int>(ZOOM_RANGE));
				break;
      case Qt::Key_H:
				home();
				break;
      case Qt::Key_R:
				QTextBrowser::reload();
				break;
      default:
        QTextBrowser::keyPressEvent(event);
      break;
    }
  }
  else
  {
    QTextBrowser::keyPressEvent(event);
  }
}

void karin::text_browser::openUrl(const QString &url)
{
	if(engine && !engine -> chmfile().isEmpty())
	{
		QTextBrowser::clear();
		this -> setSource(url);
		QTextBrowser::reload();
	}
}

QVariant karin::text_browser::loadResource(int type, const QUrl &name)
{
	if(engine && !engine -> chmfile().isEmpty())
	{
		QString path = name.toString(QUrl::StripTrailingSlash);
		int pos = path.lastIndexOf('#');
		if(pos != -1)
			path = path.left(pos);
		path = karin::chm_engine::absoluteUrl(QTextBrowser::source().toString(), path);
		if (type == QTextDocument::HtmlResource || type == QTextDocument::StyleSheetResource)
		{
			QString data;
			engine -> getFileContentAsString(&data, path);
			return QVariant(data);
		}
		else if (type == QTextDocument::ImageResource)
		{
			QByteArray data;
			QImage img;
			if(engine -> getFileContentAsBinary(&data, path))
				if(img.loadFromData(data))
					return QVariant(img);
			return QVariant(QImage());
		}
		else
			return QTextBrowser::loadResource(type, name);
	}
	else
		return QVariant();
}

void karin::text_browser::home()
{
	if(engine && !engine -> chmfile().isEmpty())
	{
		QString home = engine -> homeUrl();
		if(!home.isEmpty())
		{
			openUrl(home);
			return;
		}
		else
		{
			home = engine -> dirlist() -> front().url;
			if(!home.isEmpty())
			{
				openUrl(home);
				return;
			}
			else
				QTextBrowser::home();
		}
	}
}

void karin::text_browser::setSource(const QUrl &url)
{
	if(!engine)
		return;
	QString nurl = karin::chm_engine::absoluteUrl(QTextBrowser::source().toString(), url.toString());
	int pos = nurl.lastIndexOf('#');
	QString nnurl = nurl;
	if(pos != -1)
		nnurl = nurl.left(pos);
	nnurl = karin::decodeUrl(nnurl);
#ifdef _SIMLATOR
	qDebug()<<url<<"____"<<nurl;
#endif
	if(engine -> hasFile(nnurl))
	{
		QTextBrowser::setSource(QUrl(karin::decodeUrl(nurl)));
		return;
	}
	QVariantMap uu;
	karin::chm_engine::URLFormat format = engine -> getURLFormat(url.toString(), &uu, true);
	MApplication *app = 0;
	switch(format)
	{
		case karin::chm_engine::RemoteURL:
			if(karin::ut::Instance() -> getSetting<bool>(HANDLE_EXTERNAL_URL))
				QDesktopServices::openUrl(uu["url"].toString());
			break;
		case karin::chm_engine::NewChmURL:
			if(karin::ut::Instance() -> getSetting<bool>(HANDLE_NEW_CHM))
				if(!uu["file"].toString().isEmpty())
				{
					app = MApplication::instance();
					if(app)
						QProcess::startDetached(app -> applicationFilePath(), QStringList()<<uu["file"].toString()<<uu["page"].toString());
				}
			break;
		case karin::chm_engine::InternalURL_or_OthersURL:
			if(karin::ut::Instance() -> getSetting<bool>(HANDLE_EXTERNAL_URL))
				if(!uu["url"].toUrl().toString().isEmpty())
					QDesktopServices::openUrl(uu["url"].toUrl().toString());
		default:
			break;
	}
}

void karin::text_browser::init()
{
	QTextBrowser::clear();
	QTextBrowser::clearHistory();
}

int karin::text_browser::getVScrollBarPosition() const
{
	return QTextBrowser::verticalScrollBar() -> sliderPosition();
}

void karin::text_browser::setVScrollBarPosition( int pos )
{
	QTextBrowser::verticalScrollBar() -> setSliderPosition(pos);
}

int karin::text_browser::getHScrollBarPosition() const
{
	return QTextBrowser::horizontalScrollBar() -> sliderPosition();
}

void karin::text_browser::setHScrollBarPosition( int pos )
{
	QTextBrowser::horizontalScrollBar() -> setSliderPosition(pos);
}

void karin::text_browser::mouseMoveEvent(QMouseEvent *e)
{
	if(slide_mode && mouse_pressed)
	{
		int delta_x = e -> x() - last_x;
		int delta_y = e -> y() - last_y;
		double arg = karin::ComputeAngle(delta_x, delta_y);
		double limit = (double)(karin::ut::Instance() -> getSetting<int>(SLIDE_IGNORE_ANGLE));
#ifdef _SIMLATOR
		qDebug()<<arg;
#endif
		if((arg > 90.0 - limit && arg < 90.0 + limit) || (arg > 270.0 - limit && arg < 270.0 + limit))
			QTextBrowser::verticalScrollBar() -> setSliderPosition(QTextBrowser::verticalScrollBar() -> sliderPosition() - delta_y);
		else if((arg >= 0.0 && arg < limit) || (arg > 180.0 - limit && arg < 180.0 + limit) || (arg > 360.0 - limit && arg <= 360.0))
			QTextBrowser::horizontalScrollBar() -> setSliderPosition(QTextBrowser::horizontalScrollBar() -> sliderPosition() - delta_x);
		else
		{
			QTextBrowser::verticalScrollBar() -> setSliderPosition(QTextBrowser::verticalScrollBar() -> sliderPosition() - delta_y);
			QTextBrowser::horizontalScrollBar() -> setSliderPosition(QTextBrowser::horizontalScrollBar() -> sliderPosition() - delta_x);
		}
		last_x = e -> x();
		last_y = e -> y();
	}
	else
	{
		QTextBrowser::mouseMoveEvent(e);
	}
}

void karin::text_browser::mousePressEvent(QMouseEvent *e)
{
	if(slide_mode)
	{
		start_x = e -> x();
		start_y = e -> y();
		last_x = e -> x();
		last_y = e -> y();
		mouse_pressed = true;
	}
	QTextBrowser::mousePressEvent(e);
}

void karin::text_browser::mouseReleaseEvent(QMouseEvent *e)
{
	if(slide_mode)
	{
		if(start_x == e -> x() && start_y == e -> y())
			QTextBrowser::mouseReleaseEvent(e);
		last_x = -1;
		last_y = -1;
		start_x = -1;
		start_y = -1;
		mouse_pressed = false;
	}
	else
		QTextBrowser::mouseReleaseEvent(e);
}

void karin::text_browser::setSlideMode(bool b)
{
	if(slide_mode != b)
	{
		slide_mode = b;
		last_x = -1;
		last_y = -1;
		start_x = -1;
		start_y = -1;
		mouse_pressed = false;
		emit slideModeChanged(slide_mode);
	}
}

bool karin::text_browser::slideMode() const
{
	return slide_mode;
}

#include <QDebug>
void karin::text_browser::mouseDoubleClickEvent(QMouseEvent *e)
{
	if(slide_mode && karin::ut::Instance() -> getSetting<bool>(DOUBLE_CLICK_ZOOM))
	{
		int delta_x = QTextBrowser::width() / 2 - e -> x();
		int delta_y = QTextBrowser::height() / 2 - e -> y();
		QTextBrowser::verticalScrollBar() -> setSliderPosition(QTextBrowser::verticalScrollBar() -> sliderPosition() - delta_y);
		QTextBrowser::horizontalScrollBar() -> setSliderPosition(QTextBrowser::horizontalScrollBar() -> sliderPosition() - delta_x);

		if(zoom_dir)
		{
			QTextBrowser::zoomIn(karin::ut::Instance() -> getSetting<int>(DOUBLE_CLICK_ZOOM_RANGE));
			zoom_dir = false;
		}
		else
		{
			QTextBrowser::zoomOut(karin::ut::Instance() -> getSetting<int>(DOUBLE_CLICK_ZOOM_RANGE));
			zoom_dir = true;
		}
	}
	else
		QTextBrowser::mouseDoubleClickEvent(e);
}
