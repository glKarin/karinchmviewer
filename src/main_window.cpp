#include "main_window.h"
#include "file_chooser.h"
#include "chm_viewer.h"
#include "record_manager.h"
#include "home.h"
#include "karin_ut.h"
#include "setting_control.h"

#include <MToolBar>

karin::main_window::main_window(const QString &file, const QString &page, QWidget *parent)
	:MApplicationWindow(parent),
	filechooser(0),
	chmviewer(0),
	homepage(0),
	history(0),
	favorite(0),
	setting(0)
{
	int id = karin::ut::Instance() -> getSetting<int>(ORIENTATION);
	changeOrientation(id);
	setToolbarViewType(MToolBar::tabType);
	if(file.isEmpty())
	{
		homepage = new karin::home;
		connect(homepage, SIGNAL(openFileChooser()), this, SLOT(openFileChooser()));
		connect(homepage, SIGNAL(openHistoryList()), this, SLOT(openHistoryList()));
		connect(homepage, SIGNAL(openFavoriteList()), this, SLOT(openFavoriteList()));
		connect(homepage, SIGNAL(openSettingControl()), this, SLOT(openSettingControl()));
		homepage -> appear(this);
	}
	else
	{
		chmviewer = new karin::chm_viewer;
		chmviewer -> openChm(file, page);
		chmviewer -> appear(this);
	}
}

karin::main_window::~main_window()
{
	delete filechooser;
	delete chmviewer;
	delete homepage;
	delete history;
	delete favorite;
	delete setting;
}

void karin::main_window::openChm(const QString &file)
{
	if(currentPage())
		currentPage() -> disappear();
	if(!chmviewer)
		chmviewer = new karin::chm_viewer;
	if(chmviewer -> openChm(file))
		chmviewer -> appear(this);
}

void karin::main_window::openFileChooser()
{
	if(!filechooser)
	{
		filechooser = new karin::file_chooser;
		connect(filechooser, SIGNAL(openChm(const QString &)), this, SLOT(openChm(const QString &)));
	}
	filechooser -> appear(this);
}

void karin::main_window::openHistoryList()
{
	if(!history)
	{
		history = new karin::record_manager(HISTORY_TABLE);
		history -> setTitle(tr("History files"));
		connect(history, SIGNAL(openChm(const QString &)), this, SLOT(openChm(const QString &)));
	}
	history -> init();
	history -> appear(this);
}

void karin::main_window::openFavoriteList()
{
	if(!favorite)
	{
		favorite = new karin::record_manager(FAVORITE_TABLE);
		favorite -> setTitle(tr("Your favorites"));
		connect(favorite, SIGNAL(openChm(const QString &)), this, SLOT(openChm(const QString &)));
	}
	favorite -> init();
	favorite -> appear(this);
}

void karin::main_window::openSettingControl()
{
	if(!setting)
	{
		setting = new karin::setting_control;
		connect(setting, SIGNAL(changeOrientation(int)), this, SLOT(changeOrientation(int)));
		connect(setting, SIGNAL(setFullscreen(bool)), this, SLOT(setWindowState(bool)));
	}
	setting -> init();
	setting -> appear(this);
}

void karin::main_window::changeOrientation(int id)
{
	switch(id)
	{
		case 1:
			setPortraitOrientation();
			lockOrientation();
			break;
		case 2:
			setLandscapeOrientation();
			lockOrientation();
			break;
		case 0:
		default:
			if(isOrientationLocked())
				unlockOrientation();
			break;
	}
	karin::ut::Instance() -> setSetting<int>(ORIENTATION, id);
}

void karin::main_window::setWindowState(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(FULL_SCREEN, b);
	doShow();
}

void karin::main_window::doShow()
{
	if(karin::ut::Instance() -> getSetting<bool>(FULL_SCREEN))
		showFullScreen();
	else
		showNormal();
}
