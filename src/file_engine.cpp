#include "file_engine.h"

#include <QVariant>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#ifdef _SIMLATOR
#include <QDebug>
#endif

#define KB 1024
#define MB (KB * KB)
#define GB (MB * KB)

karin::file_engine::file_engine(QObject *object)
	:QObject(object),
	cpp_path(QDir::homePath()),
	cpp_showHidden(false),
	watcher(new QFileSystemWatcher(this))
{
	init();
	connect(watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(doDirectoryChanged(const QString &)));
}

karin::file_engine::~file_engine()
{
}

void karin::file_engine::init()
{
	cpp_filelist.clear();
	static const QDir::SortFlags flags = QDir::DirsFirst | QDir::Name | QDir::IgnoreCase;
	static const QDir::Filters qfilters = QDir::Files | QDir::AllDirs | QDir::Drives |  QDir::NoDot;
	QDir::Filters f = qfilters;
	if(cpp_showHidden)
		f |= QDir::Hidden;
	QFileInfoList list = QDir(cpp_path).entryInfoList(cpp_filters, f, flags);
	for(QFileInfoList::const_iterator itor = list.begin();
			itor != list.end();
			++itor)
	{
		if(cpp_path == "/" && itor -> fileName() == "..")
			continue;
		karin::file_info info;
		info.name = itor -> isDir() ? itor -> fileName() + "/" : itor -> fileName();
		info.path = itor -> absoluteFilePath();
		info.owner = itor -> owner();
		info.group = itor -> group();
		info.size = karin::file_engine::size2FormatString(itor -> size(), itor -> isDir());
		info.permission = karin::file_engine::permissionCode2String(itor -> permissions());
		info.type = itor -> isDir();
		cpp_filelist.push_back(info);
	}
	emit filelistChanged();
}

QString karin::file_engine::size2FormatString(qint64 size, bool isdir)
{
	QString result;
	QString p("%1%2");
	if(isdir)
	{
		if(size <= 4 * KB)
			result = p.arg(size).arg('b');
		else if(size > 4 * KB && size <= 64 * KB)
			result = p.arg(size / KB).arg('k');
	}
	else
	{
		QVariant var(size);
		double d = var.value<double>();
		if(size > 2 * KB && size < 2 * MB){
			result = p.arg(d / KB, 1, 'f', 1).arg('k');
		}
		else if(size >= 2 * MB && size < GB){
			result = p.arg(d / MB, 1, 'f', 1).arg('M');
		}
		else if(size <= 2 * KB){
			result = p.arg(size).arg('b');
		}else{
			result = p.arg(d / GB, 1, 'f', 1).arg('G');
		}
	}
	return result;
}

void karin::file_engine::setPath(const QString &path)
{
	//QString cleanpath = QDir::cleanPath(path);
	if(cpp_path != path)
	{
		QFileInfo info(path);
		while(!info.isDir())
		{
			QString s = info.absolutePath();
			if(s.isEmpty())
				s = "/";
			info.setFile(s);
		}
		cpp_path = info.absoluteFilePath();
		QStringList list = watcher -> directories();
		if(list.size() > 0)
			watcher -> removePaths(list);
		watcher -> addPath(cpp_path);
		init();
		emit pathChanged();
	}
}

void karin::file_engine::setFilters(const QStringList &list)
{
	if(cpp_filters != list)
	{
		cpp_filters = list;
		init();
	}
}

QString karin::file_engine::permissionCode2String(QFile::Permissions per)
{
	QString result("---------");
	if(per & 0x4000)
		result.replace(0, 1, "r");
	if(per & 0x2000)
		result.replace(1, 1, "w");
	if(per & 0x1000)
		result.replace(2, 1, "x");
	if(per & 0x0040)
		result.replace(3, 1, "r");
	if(per & 0x0020)
		result.replace(4, 1, "w");
	if(per & 0x0010)
		result.replace(5, 1, "x");
	if(per & 0x0004)
		result.replace(6, 1, "r");
	if(per & 0x0002)
		result.replace(7, 1, "w");
	if(per & 0x0001)
		result.replace(8, 1, "x");
	return result;
}

void karin::file_engine::setShowHidden(bool b)
{
	if(cpp_showHidden != b)
	{
		cpp_showHidden = b;
		init();
	}
}

void karin::file_engine::doDirectoryChanged(const QString &path)
{
	if(path == cpp_path)
	{
		cpp_path.clear();
		setPath(path);
	}
}
