#include "chm_viewer.h"
#include "chm_engine.h"
#include "page_view.h"
#include "directory_tree.h"
#include "index_list.h"
#include "search_list.h"
#include "karin_std.h"
#include "karin_ut.h"

#include <MContainer>
#include <MBanner>
#include <QActionGroup>
#ifdef _SIMLATOR
#include <QDebug>
#endif

karin::tab_action::tab_action(QObject *parent)
	:MAction(parent),
	tab_widget(0)
{
	connect(this, SIGNAL(triggered(bool)), this, SLOT(doTrigger(bool)));
}

karin::tab_action::tab_action(const QString &title, QObject *parent)
	:MAction(title, parent),
	tab_widget(0)
{
	connect(this, SIGNAL(triggered(bool)), this, SLOT(doTrigger(bool)));
}

karin::tab_action::tab_action(const QString &iconId, const QString &title, QObject *parent)
	:MAction(iconId, title, parent),
	tab_widget(0)
{
	connect(this, SIGNAL(triggered(bool)), this, SLOT(doTrigger(bool)));
}

karin::tab_action::~tab_action()
{
	if(tab_widget)
		delete tab_widget;
	tab_widget = 0;
}

void karin::tab_action::setTabWidget(QGraphicsWidget *widget, const QString &title)
{
	tab_widget = widget;
	tab_title = title;
}

void karin::tab_action::doTrigger(bool checked)
{
	if(checked && tab_widget)
		emit showTabWidget(tab_widget, tab_title);
}

karin::chm_viewer::chm_viewer(QGraphicsItem *parent)
	:MApplicationPage(parent),
	engine(new chm_engine(this)),
	central_widget(new MContainer),
	pageView(new tab_action(tr("Page"))),
	dirlistView(new tab_action(tr("Content"))),
	indexlistView(new tab_action(tr("Index"))),
	searchView(new tab_action(tr("Search"))),
	page(new karin::page_view),
	directory(new karin::directory_treeview),
	index(new karin::index_listview),
	search(new karin::search_listview)
{
	QActionGroup *group = new QActionGroup(this);
	setTitle(tr("Chm Viewer"));
	setPannable(false);
	page -> setChmEngine(engine);
	directory -> setModel(engine -> directoryModel());
	index -> setModel(engine -> indexModel());
	search -> setChmEngine(engine);
	pageView -> setTabWidget(page, tr("Page Viewer"));
	dirlistView -> setTabWidget(directory, tr("Content Tree"));
	indexlistView -> setTabWidget(index, tr("Index List"));
	searchView -> setTabWidget(search, tr("Search Query"));
	connect(pageView, SIGNAL(showTabWidget(QGraphicsWidget *, const QString &)), this, SLOT(changeCentralWidget(QGraphicsWidget *, const QString &)));
	connect(dirlistView, SIGNAL(showTabWidget(QGraphicsWidget *, const QString &)), this, SLOT(changeCentralWidget(QGraphicsWidget *, const QString &)));
	connect(indexlistView, SIGNAL(showTabWidget(QGraphicsWidget *, const QString &)), this, SLOT(changeCentralWidget(QGraphicsWidget *, const QString &)));
	connect(searchView, SIGNAL(showTabWidget(QGraphicsWidget *, const QString &)), this, SLOT(changeCentralWidget(QGraphicsWidget *, const QString &)));
	//connect(group, SIGNAL(triggered(QAction *)), this, SLOT(changeCentralWidget(QAction *)));
	group -> setExclusive(true);
	clearActions();
	pageView -> setLocation(MAction::ToolBarLocation);
	dirlistView -> setLocation(MAction::ToolBarLocation);
	indexlistView -> setLocation(MAction::ToolBarLocation);
	searchView -> setLocation(MAction::ToolBarLocation);
	pageView -> setActionGroup(group);
	dirlistView -> setActionGroup(group);
	indexlistView -> setActionGroup(group);
	searchView -> setActionGroup(group);
	pageView -> setCheckable(true);
	dirlistView -> setCheckable(true);
	indexlistView -> setCheckable(true);
	searchView -> setCheckable(true);
	addAction(pageView);
	addAction(dirlistView);
	addAction(indexlistView);
	addAction(searchView);
	connect(directory, SIGNAL(openUrl(const QString &)), this, SLOT(openUrl(const QString &)));
	connect(index, SIGNAL(openUrl(const QString &)), this, SLOT(openUrl(const QString &)));
	connect(search, SIGNAL(openUrl(const QString &)), this, SLOT(openUrl(const QString &)));
	connect(engine, SIGNAL(output(int, const QString &)), this, SLOT(showInfoBanner(int, const QString &)));
	connect(page, SIGNAL(addFavorite()), this, SLOT(addFavorite()));
	setCentralWidget(central_widget);
}

karin::chm_viewer::~chm_viewer()
{
	delete pageView;
	delete dirlistView;
	delete indexlistView;
	delete searchView;
}

bool karin::chm_viewer::openChm(const QString &file, const QString &p)
{
#ifdef _SIMLATOR
	qDebug()<<file;
#endif
	karin::OperationResult res = engine -> loadFile(file);
	switch(res)
	{
		case karin::Successful:
			setTitle(engine -> title() + " - " + "Karin Chm Viewer");
			page -> init(true);
			directory -> init(engine -> hasTableOfContents());
			index -> init(engine -> hasIndexTable());
			search -> init(engine -> hasSearchTable());
			if(!p.isEmpty())
				page -> openUrl(p);
			karin::ut::Instance() -> doRecord(HISTORY_TABLE, engine -> title().isEmpty() ? tr("CHM File") : engine -> title(), engine -> chmfile());
			break;
		case karin::Failed:
			setTitle("Karin Chm Viewer");
			page -> init(false);
			directory -> init(false);
			index -> init(false);
			search -> init(false);
			break;
		case karin::Nothing:
			break;
		default:
			break;
	}
	pageView -> setChecked(true);
	pageView -> trigger();
	return res != karin::Failed;
}

void karin::chm_viewer::changeCentralWidget(QAction *action)
{
	karin::tab_action *tac = dynamic_cast<karin::tab_action *>(action);
	if(tac)
	{
		central_widget -> setCentralWidget(tac -> tab_widget, false);
		central_widget -> setTitle(tac -> tab_title);
	}
}

void karin::chm_viewer::changeCentralWidget(QGraphicsWidget *widget, const QString &title)
{
	if(widget && central_widget -> centralWidget() != widget)
	{
		central_widget -> setCentralWidget(widget, false);
		central_widget -> setTitle(title);
	}
}

void karin::chm_viewer::openUrl(const QString &url)
{
	page -> openUrl(url);
	pageView -> setChecked(true);
	pageView -> trigger();
}

void karin::chm_viewer::showInfoBanner(int t, const QString &info)
{
	MBanner *infoBanner = new MBanner;
	karin::OutputType type = static_cast<karin::OutputType>(t);
	infoBanner -> setTitle(karin::getOutputTypeString(type));
	infoBanner -> setSubtitle(info);
	infoBanner -> setIconID(karin::getOutputTypeIconId(type));
	infoBanner -> appear(MSceneWindow::DestroyWhenDone);
}

void karin::chm_viewer::addFavorite()
{
	if(engine -> chmfile().isEmpty())
		return;
	bool b = karin::ut::Instance() -> isFavoriteChm(engine -> chmfile());
	if(b)
	{
		karin::ut::Instance() -> removeOneRecord(FAVORITE_TABLE, RCD_VALUE, engine -> chmfile());
	}
	else
	{
		karin::ut::Instance() -> doRecord(FAVORITE_TABLE, engine -> title().isEmpty() ? tr("CHM File") : engine -> title(), engine -> chmfile());
	}
	page -> checkFavorite();
	showInfoBanner(0, b ? tr("Remove favorite successful") : tr("Add to favorite successful"));
}

