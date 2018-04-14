#include "page_view.h"
#include "text_browser.h"
#include "chm_engine.h"
#include "toolbar.h"
#include "encode_popup.h"
#include "bookmark_dialog.h"
#include "karin_ut.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>
#include <MSlider>
#include <MScene>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <MAction>
#include <MSeparator>

karin::page_view::page_view(QGraphicsItem *parent)
	:MContainer(parent),
	slide_or_select_mode(karin::page_view::select_mode),
	engine(0),
	textbrowser(new text_browser),
	label(new MLabel),
	vslider(new MSlider),
	hslider(new MSlider),
	toolbar(new karin::tool_bar),
	zoomin(new MAction("icon-m-common-add", tr("Zoom In"), this)),
	zoomout(new MAction("icon-m-common-remove", tr("Zoom Out"), this)),
	home(new MAction("icon-m-toolbar-home",tr("Home"), this)),
	prev(new MAction("icon-m-toolbar-previous",tr("Privious"), this)),
	next(new MAction("icon-m-toolbar-next",tr("Next"), this)),
	reload(new MAction("icon-m-toolbar-refresh", tr("Refresh"), this)),
	codec(new MAction("icon-m-toolbar-list",tr("Encode"), this)),
	mark(new MAction("icon-m-toolbar-directory-move-to",tr("BookMark"), this)),
	add(new MAction(this)),
	slider(new MAction(this)),
	copy(new MAction("icon-m-toolbar-edit",tr("Copy"), this)),
	bookmark(new karin::bookmark_dialog)
{
	MLayout *mainlayout = new MLayout;
	MLayout *sublayout = new MLayout;
	mainpolicy = new MLinearLayoutPolicy(mainlayout, Qt::Vertical);
	subpolicy = new MLinearLayoutPolicy(sublayout, Qt::Horizontal);
	setHeaderVisible(false);
	mainlayout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	sublayout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	label -> setTextElide(true);
	label -> setAlignment(Qt::AlignCenter);
	vslider -> setOrientation(Qt::Vertical);
	zoomin -> setLocation(MAction::ToolBarLocation);
	zoomout -> setLocation(MAction::ToolBarLocation);
	home -> setLocation(MAction::ToolBarLocation);
	prev -> setLocation(MAction::ToolBarLocation);
	next -> setLocation(MAction::ToolBarLocation);
	reload -> setLocation(MAction::ToolBarLocation);
	codec -> setLocation(MAction::ToolBarLocation);
	mark -> setLocation(MAction::ToolBarLocation);
	add -> setLocation(MAction::ToolBarLocation);
	toolbar -> addAction(prev);
	toolbar -> addAction(next);
	toolbar -> addAction(slider);
	toolbar -> addAction(copy);
	toolbar -> addAction(zoomin);
	toolbar -> addAction(zoomout);
	toolbar -> addAction(reload);
	toolbar -> addAction(mark);
	toolbar -> addAction(home);
	toolbar -> addAction(codec);
	toolbar -> addAction(add);
	connect(zoomin, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
	connect(zoomout, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
	connect(prev, SIGNAL(triggered(bool)), textbrowser, SLOT(backward()));
	connect(next, SIGNAL(triggered(bool)), textbrowser, SLOT(forward()));
	connect(home, SIGNAL(triggered(bool)), textbrowser, SLOT(home()));
	connect(add, SIGNAL(triggered(bool)), this, SIGNAL(addFavorite()));
	connect(slider, SIGNAL(triggered(bool)), this, SLOT(setSlideSelectMode()));
	connect(copy, SIGNAL(triggered(bool)), textbrowser, SLOT(copy()));
	connect(textbrowser, SIGNAL(copyAvailable(bool)), copy, SLOT(setEnabled(bool)));
	connect(reload, SIGNAL(triggered(bool)), textbrowser, SLOT(reload()));
	connect(codec, SIGNAL(triggered(bool)), this, SLOT(openEncodeDialog()));
	connect(mark, SIGNAL(triggered(bool)), this, SLOT(openBookmarkDialog()));
	connect(hslider, SIGNAL(valueChanged(int)), this, SLOT(scrollHbar(int)));
	connect(vslider, SIGNAL(valueChanged(int)), this, SLOT(scrollVbar(int)));
	connect(textbrowser, SIGNAL(scrollHBarValueChanged(int)), this, SLOT(slideHBar(int)));
	connect(textbrowser, SIGNAL(scrollVBarValueChanged(int)), this, SLOT(slideVBar(int)));
	connect(textbrowser, SIGNAL(scrollHBarRangeChanged(int, int)), hslider, SLOT(setRange(int, int)));
	connect(textbrowser, SIGNAL(scrollVBarRangeChanged(int, int)), vslider, SLOT(setRange(int, int)));
	connect(textbrowser, SIGNAL(backwardAvailable(bool)), prev, SLOT(setEnabled(bool)));
	connect(textbrowser, SIGNAL(forwardAvailable(bool)), next, SLOT(setEnabled(bool)));
	connect(textbrowser, SIGNAL(sourceChanged(const QUrl &)), this, SLOT(setLabelIsBuddy(const QUrl &)));
	/*
	MScene scene;
	QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(scene.addWidget(textbrowser));
	if(widget)
		subpolicy -> addItem(widget);
		*/
	QGraphicsProxyWidget *widget = new QGraphicsProxyWidget;
	widget -> setWidget(textbrowser);
	subpolicy -> addItem(widget);
	//subpolicy -> addItem(vslider);
	mainpolicy -> addItem(label);
	mainpolicy -> addItem(sublayout);
	//mainpolicy -> addItem(hslider);
	mainpolicy -> addItem(new MSeparator);
	mainpolicy -> addItem(toolbar);
	connect(bookmark, SIGNAL(openUrl(const QString &)), this, SLOT(openUrl(const QString &)));
	setSlideSelectMode();
	centralWidget() -> setLayout(mainlayout);
}

karin::page_view::~page_view()
{
	engine = 0;
	delete bookmark;
}

void karin::page_view::setChmEngine(karin::chm_engine *e)
{
	engine = e;
	textbrowser -> setChmEngine(e);
}

void karin::page_view::openUrl(const QString &url)
{
	if(engine && !engine -> chmfile().isEmpty())
	{
		QString nurl;
		if(url.isEmpty())
			textbrowser -> home();
		else
			textbrowser -> openUrl(url);
	}
}

void karin::page_view::zoomIn()
{
	textbrowser -> zoomIn(karin::ut::Instance() -> getSetting<int>(ZOOM_RANGE));
}

void karin::page_view::zoomOut()
{
	textbrowser -> zoomOut(karin::ut::Instance() -> getSetting<int>(ZOOM_RANGE));
}

void karin::page_view::slideHBar(int value)
{
	if(value != hslider -> value())
	{
		hslider -> setValue(value);
	}
}

void karin::page_view::slideVBar(int value)
{
	int newvalue = vslider -> maximum() - vslider -> value();
	if(value != newvalue)
	{
		vslider -> setValue(vslider -> maximum() - value);
	}
}

void karin::page_view::scrollHbar(int value)
{
	if(value != textbrowser -> getHScrollBarPosition())
	{
		textbrowser -> setHScrollBarPosition(value);
	}
}

void karin::page_view::scrollVbar(int value)
{
	int newvalue = vslider -> maximum() - value;
	if(newvalue != textbrowser -> getVScrollBarPosition())
	{
		textbrowser -> setVScrollBarPosition(newvalue);
	}
}

void karin::page_view::setLabelIsBuddy(const QUrl &url)
{
	QString path = url.toString();
	label -> setText(engine -> getTopicByUrl(path));
}

void karin::page_view::init(bool success)
{
	textbrowser -> init();
	zoomin -> setEnabled(success);
	zoomout -> setEnabled(success);
	home -> setEnabled(success);
	reload -> setEnabled(success);
	codec -> setEnabled(success);
	add -> setEnabled(success);
	slider -> setEnabled(success);
	copy -> setEnabled(false);
	mark -> setEnabled(success);
	vslider -> setEnabled(success);
	hslider -> setEnabled(success);
	if(success)
	{
		textbrowser -> home();
		checkFavorite();
	}
}

void karin::page_view::openEncodeDialog()
{
	if(engine && !engine -> chmfile().isEmpty())
	{
		karin::encode_popup *encode = new karin::encode_popup;
		connect(encode, SIGNAL(chooseEncode(const QString &)), this, SLOT(chooseEncode(const QString &)));
		encode -> reset(engine -> codec());
		encode -> appear(MSceneWindow::DestroyWhenDone);
	}
}

void karin::page_view::chooseEncode(const QString &e)
{
	if(engine -> setTextCodec(e))
		textbrowser -> reload();
}

void karin::page_view::openBookmarkDialog()
{
	if(engine && !engine -> chmfile().isEmpty())
	{
		QString url = textbrowser -> source().toString();
		bookmark -> init(engine -> chmfile(), engine -> getTopicByUrl(url), url);
		bookmark -> appear();
	}
}

void karin::page_view::setSlideSelectMode()
{
	slide_or_select_mode = slide_or_select_mode == karin::page_view::slide_mode ? karin::page_view::select_mode : karin::page_view::slide_mode;
	if(slide_or_select_mode == karin::page_view::slide_mode)
	{
		if(subpolicy -> indexOf(vslider) != -1)
			subpolicy -> removeItem(vslider);
		if(mainpolicy -> indexOf(hslider) != -1)
			mainpolicy -> removeItem(hslider);
		label -> setPreferredHeight(40);
		label -> setMaximumHeight(40);
		label -> setMinimumHeight(40);
		slider -> setIconID("icon-m-toolbar-callhistory");
		slider -> setText(tr("Slide Mode"));
	}
	else
	{
		if(subpolicy -> indexOf(vslider) == -1)
			subpolicy -> addItem(vslider);
		if(mainpolicy -> indexOf(hslider) == -1)
			mainpolicy -> insertItem(2, hslider);
		label -> setPreferredHeight(20);
		label -> setMaximumHeight(20);
		label -> setMinimumHeight(20);
		slider -> setIconID("icon-m-toolbar-cut-paste");
		slider -> setText(tr("Select Mode"));
	}
	textbrowser -> setSlideMode(slide_or_select_mode == karin::page_view::slide_mode);
}

void karin::page_view::checkFavorite()
{
	bool b = karin::ut::Instance() -> isFavoriteChm(engine -> chmfile());
	add -> setIconID(b ? "icon-m-toolbar-favorite-mark" : "icon-m-toolbar-favorite-unmark");
	add -> setText(b ? tr("Remove Favorite") : tr("Add Favorite"));
}
