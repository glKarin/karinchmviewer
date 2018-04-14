#include "main_window.h"
#include "karin_service.h"
#include "karin_ut.h"

#include <MApplication>
#include <MComponentCache>
//#include <QCleanlooksStyle>
#include <QPlastiqueStyle>
#include <QLocale>
#include <QTranslator>
#include <QScopedPointer>
#include <QSplashScreen>
#include <QTextCodec>
#include <MTheme>
//#include <QTimer>

#include <QDebug>

//#define SPLASH_TIMEOUT 2000

int main(int argc, char *argv[])
{
	QScopedPointer<MApplication> app(MComponentCache::mApplication(argc, argv, DEVELOPER, new karin::karin_service(M_APPLICATION_NAME)));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	app -> setApplicationName(M_APPLICATION_NAME);
	app -> setOrganizationName(DEVELOPER);
	app -> setApplicationVersion(VERSION);
	if(MTheme::instance() -> loadCSS(QString(KARIN_PREFIX) + "/style/karin_mcontentitemstyle.css"))
		qDebug()<<"load stylesheet -> MContentItem";
	else
		qWarning()<<"stylesheet missing -> MContentItem";
	QTranslator translator;
#ifdef _SIMLATOR
	if(translator.load(QString("karinchmviewer.zh_CN.qm"),"i18n/"))
	{
		qDebug()<<"Load i18n -> \"karinchmviewer.zh_CN.qm\"";
#else
	QString locale = QLocale::system().name();

	if(translator.load(QString("karinchmviewer.") + locale, QString(KARIN_PREFIX) + "/i18n/"))
	{
		qDebug()<<(QString("Load i18n -> \"karinchmviewer.") + locale + ".qm\"");
#endif
		app -> installTranslator(&translator);
	}

	MApplication::setStyle( new QPlastiqueStyle );
	if(argc > 1)
	{
		QString file(argv[1]);
		qDebug()<<QObject::tr("Opening chm file ") + " -> " + file;
		QString page;
		if(argc > 2)
		{
			page = QString(argv[2]);
			qDebug()<<QObject::tr("Opening html page ") + " -> " + page;
		}
		karin::main_window window(file, page);
		window.doShow();
		return app -> exec();
	}
	karin::main_window window;
	if(karin::ut::Instance() -> getSetting<bool>(SHOW_SPLASH))
	{
		QSplashScreen *splash = new QSplashScreen(QPixmap(QString(KARIN_PREFIX) + "/splash/karinchmviewer_splash.jpg"));
		splash -> show();
		splash -> raise();
		splash -> finish(&window);
		splash -> deleteLater();
	}
	window.doShow();
	return app -> exec();
}
