#include "home.h"
#include "karin_std.h"
#include "header_view.h"
#include "about_dialog.h"

#include <MContentItem>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MPannableViewport>
#include <QImage>
#include <QGraphicsAnchorLayout>
#include <MWidget>
#include <MApplication>

karin::home::home(QGraphicsItem *parent)
	:MApplicationPage(parent)
{
	karin::header_view *header = new karin::header_view;
	karin::header_view *footer = new karin::header_view(karin::header_view::footerType);
	MPannableViewport *pan = new MPannableViewport;
	MWidget *widget = new MWidget;
	QGraphicsAnchorLayout *mainLayout = new QGraphicsAnchorLayout;
	MLayout *menuLayout = new MLayout;
	MLinearLayoutPolicy *menuPolicy = new MLinearLayoutPolicy(menuLayout, Qt::Vertical);
	MContentItem *filechooser = new MContentItem;
	MContentItem *history = new MContentItem;
	MContentItem *favorite = new MContentItem;
	MContentItem *setting = new MContentItem;
	MContentItem *about = new MContentItem;
	MContentItem *exit = new MContentItem;
	setTitle("Karin CHM Viewer");
	setPannable(false);
	header -> setTitleFont(karin::getConstantFont(karin::Super, QFont::Bold));
	footer -> setTitleFont(karin::getConstantFont(karin::Large, QFont::Bold));
	header -> setTitle("Karin CHM Viewer");
	footer -> setTitle("Karin @ 2015");
	filechooser -> setTitle(tr("File Chooser"));
	filechooser -> setSubtitle(tr("Choose a chm file to view in file system"));
	filechooser -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-folder.png"));
	history -> setTitle(tr("History Files"));
	history -> setSubtitle(tr("Recording a chm file list of opened"));
	history -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-document.png"));
	favorite -> setTitle(tr("Favorites"));
	favorite -> setSubtitle(tr("Recording a chm file list of you collected"));
	favorite -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-collection.png"));
	setting -> setTitle(tr("Setting"));
	setting -> setSubtitle(tr("Application global setting"));
	setting -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-setting.png"));
	about -> setTitle(tr("About"));
	about -> setSubtitle(tr("About Karin CHM Viewer"));
	about -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-about.png"));
	exit -> setTitle(tr("Exit"));
	exit -> setSubtitle(tr("Quit Karin CHM Viewer"));
	exit -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-quit.png"));
	menuPolicy -> addItem(filechooser);
	menuPolicy -> addItem(history);
	menuPolicy -> addItem(favorite);
	menuPolicy -> addItem(setting);
	menuPolicy -> addItem(about);
	menuPolicy -> addItem(exit);
	widget -> setLayout(menuLayout);
	pan -> setWidget(widget);
	//pan -> setPreferredHeight(380);
	//pan -> setMinimumHeight(380);
	//pan -> setMaximumHeight(380);
	mainLayout -> addAnchor(header, Qt::AnchorTop, mainLayout, Qt::AnchorTop);
	mainLayout -> addAnchor(header, Qt::AnchorLeft, mainLayout, Qt::AnchorLeft);
	mainLayout -> addAnchor(header, Qt::AnchorRight, mainLayout, Qt::AnchorRight);
	mainLayout -> addAnchor(pan, Qt::AnchorTop, header, Qt::AnchorBottom);
	mainLayout -> addAnchor(pan, Qt::AnchorBottom, footer, Qt::AnchorTop);
	mainLayout -> addAnchor(pan, Qt::AnchorLeft, mainLayout, Qt::AnchorLeft);
	mainLayout -> addAnchor(pan, Qt::AnchorRight, mainLayout, Qt::AnchorRight);
	mainLayout -> addAnchor(footer, Qt::AnchorBottom, mainLayout, Qt::AnchorBottom);
	mainLayout -> addAnchor(footer, Qt::AnchorLeft, mainLayout, Qt::AnchorLeft);
	mainLayout -> addAnchor(footer, Qt::AnchorRight, mainLayout, Qt::AnchorRight);
	connect(filechooser, SIGNAL(clicked()), this, SIGNAL(openFileChooser()));
	connect(history, SIGNAL(clicked()), this, SIGNAL(openHistoryList()));
	connect(favorite, SIGNAL(clicked()), this, SIGNAL(openFavoriteList()));
	connect(setting, SIGNAL(clicked()), this, SIGNAL(openSettingControl()));
	connect(about, SIGNAL(clicked()), this, SLOT(openAboutDialog()));
	connect(exit, SIGNAL(clicked()), qApp, SLOT(quit()));
	centralWidget() -> setLayout(mainLayout);
}

karin::home::~home()
{
}

void karin::home::openAboutDialog()
{
	karin::about_dialog *msgBox = new karin::about_dialog;
	msgBox -> setTitle("Karin CHM Viewer");
	QStringList sl;
	sl<<tr("Karin CHM Viewer is a chm viewer on MeeGo Harmattan")
		<<tr("Using MeeGoTouch API and KChmViewer parse engine")
		<<""
		<<tr("Support to content tree view, index list view, search, add bookmarks, record history and add favorite")
		<<""
		<<"<b>" + tr("Update") + "</b> : "
		<<tr("1. Add slide and select in QTextBrowser desktop widget for slide gesture on mobile.")
		<<tr("2. Double-Click to zoom.")
		<<""
		<<"<b>" + tr("Version") + "</b> : " + VERSION
		<<"<b><font color=\"#9E1B29\">Karin</font></b> Zhao @ 2015"
		<<"DOSPY - <b><font color=\"#9E1B29\">香磷</font></b>ヽ"
		<<"<a href=\"mailto:beyondk2000@gmail.com\">beyondk2000@gmail.com</a>";
	msgBox -> setContentTexts(sl);
	msgBox -> setImagePath(QString(KARIN_PREFIX) + "/resources/kchmviewer.png");
	msgBox -> setTitleBarIconId("icon-l-user-guide-main-view");
	msgBox -> appear(MSceneWindow::DestroyWhenDone);
}

