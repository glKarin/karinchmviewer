#include <libchmfileimpl.h>
#include <libchmurlfactory.h>

#include <QStack>
#ifdef _SIMLATOR
#include <QDebug>
#endif
#include <QDir>
#include <QUrl>

#include "chm_engine.h"

	karin::directory_model::directory_model(QObject *parent)
:QAbstractListModel(parent),
	m_table(0)
{
}

karin::directory_model::~directory_model()
{
	m_table = 0;
}

int karin::directory_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return list.size();
}

QVariant karin::directory_model::data(const QModelIndex &index, int role) const
{
	QVariantMap data;
	if(role == Qt::DisplayRole)
	{
		int i = index.row();
		if(i >= list.size())
			return QVariant(data);
		const karin::directory_item *item = list.at(i).node;
		data.insert("name", QVariant(item -> name));
		data.insert("url", QVariant(item -> url));
		data.insert("indent", QVariant(item -> indent));
		data.insert("isdir", QVariant(item -> isdir));
		data.insert("opened", QVariant(list.at(i).opened));
	}
	return QVariant(data);
}

void karin::directory_model::open(const QModelIndex &node)
{
	if(m_table)
	{
		int i = node.row();
		const karin::directory_item *d = list.at(i).node;
		if(!d -> isdir)
			return;
		int index = d -> index;
		QString parent = d -> name;
		int indent = d -> indent;
		bool opened = list.at(i).opened;
		int l = i + 1;
		if(opened)
		{
			while(l < list.size() && list.at(l).node -> indent > indent)
			{
				list.removeAt(l);
			}
		}
		else
		{
			for(int j = index + 1; j < m_table -> size() && m_table -> at(j).indent > indent; j++)
			{
				if(m_table -> at(j).indent == indent + 1)
				{
					karin::directory_node item;
					item.node =	&(m_table -> at(j));
					item.opened = false;
					list.insert(l, item);
					l++;
				}
			}
		}
		list[i].opened = !list.at(i).opened;
		emit layoutChanged();
	}
}

void karin::directory_model::init()
{
	list.clear();
	if(m_table)
		for(QVector<const karin::directory_item>::iterator itor = m_table -> begin();
				itor != m_table -> end();
				++itor)
			if(itor -> parent == "/")
			{
				karin::directory_node item;
				item.node = &(*itor);
				item.opened = false;
				list.push_back(item);
			}
	emit layoutChanged();
}

void karin::directory_model::all()
{
	list.clear();
	if(m_table)
		for(QVector<const karin::directory_item>::iterator itor = m_table -> begin();
				itor != m_table -> end();
				++itor)
		{
			karin::directory_node item;
			if(itor -> isdir)
				item.opened = true;
			item.node = &(*itor);
			list.push_back(item);
		}
	emit layoutChanged();
}

	karin::index_model::index_model(QObject *parent)
:QAbstractListModel(parent),
	m_table(0)
{
}

karin::index_model::~index_model()
{
	m_table = 0;
}

int karin::index_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	if(m_table)
		return m_table -> size();
	else
		return 0;
}

QVariant karin::index_model::data(const QModelIndex &index, int role) const
{
	QVariantMap data;
	if(m_table)
	{
		if(role == Qt::DisplayRole)
		{
			int i = index.row();
			if(i >= m_table -> size())
				return QVariant(data);
			data.insert("name", QVariant(m_table -> at(i).name));
			data.insert("subname", QVariant(m_table -> at(i).subitem.size()));
		}
	}
	return QVariant(data);
}

	karin::chm_engine::chm_engine(QObject *object)
:QObject(object),
	LCHMFile(),
	cpp_indexModel(new karin::index_model(this)),
	cpp_directoryModel(new karin::directory_model(this))
{
	cpp_indexModel -> m_table = &cpp_indexlist;
	cpp_directoryModel -> m_table = &cpp_dirlist;
	connect(this, SIGNAL(indexlistChanged()), cpp_indexModel, SIGNAL(layoutChanged()));
	connect(this, SIGNAL(dirlistChanged()), cpp_directoryModel, SLOT(init()));
}

karin::chm_engine::~chm_engine()
{
	this -> closeAll();
}

void karin::chm_engine::init()
{
	hasTOC = LCHMFile::hasTableOfContents();
	if(!hasTOC)
	{
		doDebug(karin::DebugOutput, tr("The chm file has not a table of contents"));
	}
	else
	{
		QVector<LCHMParsedEntry> vector;
		hasTOC = LCHMFile::parseTableOfContents(&vector);
		if(!hasTOC)
		{
			doDebug(karin::WarningOutput, tr("Parsing table of contents error in this chm file"));
		}
		else
		{
			parseTOC_vector2list(vector);
		}
	}
	emit dirlistChanged();
	hasTOI = LCHMFile::hasIndexTable();
	if(!hasTOI)
	{
		doDebug(karin::DebugOutput, tr("The chm file has not a table of index"));
	}
	else
	{
		QVector<LCHMParsedEntry> vector;
		hasTOI = LCHMFile::parseIndex(&vector);
		if(!hasTOI)
		{
			doDebug(karin::WarningOutput, tr("Parsing table of index error in this chm file"));
		}else
		{
			parseTOI_vector2list(vector);
		}
	}
	emit indexlistChanged();
	cpp_codec = LCHMFile::currentEncoding() -> qtcodec;
}

karin::OperationResult karin::chm_engine::loadFile(const QString &s)
{
	if(s != cpp_chmfile)
	{
		if(!cpp_chmfile.isEmpty())
		{
			this -> closeAll();
		}
		cpp_chmfile.clear();
		if(!LCHMFile::loadFile(s))
		{
			doDebug(karin::FatalOutput, tr("Loading chm file error"));
			return karin::Failed;
		}
		cpp_chmfile = s;
		init();
		return karin::Successful;
	}
	return karin::Nothing;
}

void karin::chm_engine::closeAll()
{
	LCHMFile::closeAll();
	cpp_dirlist.clear();
	emit dirlistChanged();
	cpp_indexlist.clear();
	emit indexlistChanged();
	hasTOC = false;
	hasTOI = false;
}

void karin::chm_engine::parseTOC_vector2list(const QVector<LCHMParsedEntry> &vector)
{
	QStack<QString> parent;
	int last = 0;
	int count = 0;
	for(QVector<const LCHMParsedEntry>::const_iterator itor = vector.begin();
			itor != vector.end();
			++itor, count++)
	{
		karin::directory_item dir;
		if(parent.isEmpty())
			parent.push("/");
		if(itor + 1 != vector.end())
			last = (itor+1) -> indent;
		else
			last = 0;
		dir.name = itor->name;
		dir.indent = itor->indent;
		dir.imageid = itor->imageid;
		//dir.opened = false;
		dir.index = count;
		if(!itor -> urls.isEmpty())
			dir.url = itor->urls.at(0);
		dir.parent = parent.top();
		dir.isdir = false;
		if((*itor).indent > last)
		{
			for(int i = last; i < itor -> indent; i++)
				parent.pop();
		}
		else if((*itor).indent < last)
		{
			dir.isdir = true;
			parent.push(itor -> name);
		}
		cpp_dirlist.push_back(dir);
	}
}

bool karin::chm_engine::parseTOI_vector2list(const QVector<LCHMParsedEntry> &vector)
{
	QList<QStringList> list;
	QString src;
	QString hhk(LCHMFile::impl() -> m_indexFile);
	if ( !getFileContentAsString( &src, hhk ) || src.isEmpty() )
		return false;
	int pos = 0;
	bool in_object = false;
	QStringList sl;
	int stringlen = src.length();
	while ( pos < stringlen && (pos = src.indexOf ('<', pos)) != -1 )
	{
		int i, word_end = 0;
		for ( i = ++pos; i < stringlen; i++ )
		{
			// If a " or ' is found, skip to the next one.
			if ( (src[i] == '"' || src[i] == '\'') )
			{
				// find where quote ends, either by another quote, or by '>' symbol (some people don't know HTML)
				int nextpos = src.indexOf (src[i], i+1);
				if ( nextpos == -1 	&& (nextpos = src.indexOf ('>', i+1)) == -1 )
				{
					qWarning ("LCHMFileImpl::ParseHhcAndFillTree: corrupted TOC: %s", qPrintable( src.mid(i) ));
					return false;
				}
				i =  nextpos;
			}
			else if ( src[i] == '>'  )
				break;
			else if ( !src[i].isLetterOrNumber() && src[i] != '/' && !word_end )
				word_end = i;
		}
		QString tagword, tag = src.mid (pos, i - pos);
		if ( word_end )
			tagword = src.mid (pos, word_end - pos).toLower();
		else
			tagword = tag.toLower();
		if ( tagword == "object" && tag.indexOf ("text/sitemap", 0, Qt::CaseInsensitive ) != -1 )
			in_object = true;
		else if ( tagword == "/object" && in_object ) 
		{
			list.push_back(sl);
			sl.clear();
			in_object = false;
		}
		else if ( tagword == "param" && in_object )
		{
			// <param name="Name" value="First Page">
			int offset; // strlen("param ")
			QString name_pattern = "name=", value_pattern = "value=";
			QString pname, pvalue;
			if ( (offset = tag.indexOf (name_pattern, 0, Qt::CaseInsensitive )) == -1 )
			{
				qWarning ("LCHMFileImpl::ParseAndFillTopicsTree: bad <param> tag '%s': no name=\n", qPrintable( tag ));
				return false;
			}
			// offset+5 skips 'name='
			offset = LCHMFile::impl() -> findStringInQuotes (tag, offset + name_pattern.length(), pname, TRUE, FALSE);
			pname = pname.toLower();
			if ( (offset = tag.indexOf(value_pattern, offset, Qt::CaseInsensitive )) == -1 )
				qWarning ("LCHMFileImpl::ParseAndFillTopicsTree: bad <param> tag '%s': no value=\n", qPrintable( tag ));
			// offset+6 skips 'value='
			LCHMFile::impl() -> findStringInQuotes (tag, offset + value_pattern.length(), pvalue, FALSE, TRUE);
			//qDebug ("<param>: name '%s', value '%s'", qPrintable( pname ), qPrintable( pvalue ));
			if ( pname == "name" || pname == "keyword" )
			{
				// Some help files contain duplicate names, where the second name is empty. Work it around by keeping the first one
				if ( !pvalue.isEmpty() )
					sl.push_back(pvalue);
			}
		}
		pos = i;	
	}
	for(int i = 0; i < vector.size(); i++)
	{
		karin::index_item item;
		item.name = list.at(i).front();
		int k = list.at(i).size() - 1;
		for(int j = vector.at(i).urls.size() - 1; j >= 0 && k >= 0; j--)
		{
			if(vector.at(i).urls.at(j).startsWith(':'))
				continue;
			karin::searchresult_item sitem;
			sitem.name = list.at(i).at(k);
			sitem.url = vector.at(i).urls.at(j);
			item.subitem.push_front(sitem);
			k--;
		}
		item.imageid = vector.at(i).imageid;
		cpp_indexlist.push_back(item);
	}
	return true;
}

bool karin::chm_engine::setTextCodec(const QString &code)
{
	if(code != cpp_codec)
	{
		const LCHMTextEncoding *p = LCHMFile::impl() -> lookupByQtCodec(code);
		if(!p)
		{
#ifdef _SIMLATOR
			qWarning()<<"No so such encoding";
#endif
			return false;
		}
		if(!LCHMFile::setCurrentEncoding(p))
		{
			doDebug(karin::DebugOutput, tr("Can not set new encoding in this chm file"));
			return false;
		}
		cpp_codec = code;
		return true;
	}
	return false;
}

QVector<karin::encoding_item> karin::chm_engine::getEncodingList()
{
	QVector<karin::encoding_item> list;
	if(list.isEmpty())
		for(const LCHMTextEncoding * p = LCHMFileImpl::getTextEncodingTable();
				p -> family;
				++p)
		{
			karin::encoding_item ze;
			ze.family = p -> family;
			ze.codec = p -> qtcodec;
			list.push_back(ze);
		}
	return list;
}

bool karin::chm_engine::searchQuery(QVector<searchresult_item> *vector, const QString &key, unsigned int limit)
{
	vector -> clear();
	if(!hasSearchTable())
	{
		doDebug(karin::WarningOutput, tr("The chm file has not a table of search"));
		return false;
	}
	QStringList result;
	if(!LCHMFile::searchQuery(key, &result, limit))
	{
		doDebug(karin::DebugOutput, tr("Nothing matched with keyword") + " - \"" + key + "\"");
		return false;
	}
	for(QStringList::const_iterator itor = result.begin();
			itor != result.end();
			++itor)
	{
		searchresult_item item;
		item.name = getTopicByUrl(*itor);
		item.url = *itor;
		vector -> push_back(item);
	}
	return true;
}

QString karin::chm_engine::absoluteUrl(const QString &baseUrl_, const QString & url)
{
	QString p1;
	QString p2;
	QString newurl = url;
	QString dir;
	int i = baseUrl_.lastIndexOf('/');
	if ( i != -1 )
		dir = QDir::cleanPath(baseUrl_.left(i + 1));
	else
		dir = "/";
	if ( !LCHMUrlFactory::isRemoteURL (url, p1)
			&& !LCHMUrlFactory::isJavascriptURL (url)
			&& !LCHMUrlFactory::isNewChmURL (url,"",  p1, p2) )
	{
		newurl = QDir::cleanPath(url);
		// Normalize url, so it becomes absolute
		if ( newurl[0] != '/' )
			newurl = dir + "/" + newurl;
		newurl = QDir::cleanPath(newurl);
	}
	return newurl;
}

void karin::chm_engine::doDebug(karin::OutputType type, const QString &info)
{
#ifdef _SIMLATOR
	qDebug()<<karin::getOutputTypeString(type) + " -> " + info;
#endif
	emit output(static_cast<int>(type), info);
}

karin::chm_engine::URLFormat karin::chm_engine::getURLFormat(const QString &name, QVariantMap *uu, bool dbg)
{
	QString protocol, new_chm, new_page;
	uu -> clear();
	if(LCHMUrlFactory::isRemoteURL(name, protocol))
	{
		if(dbg)
			doDebug(karin::DebugOutput, tr("This is a") + " " + karin::getRemoteUrlName(protocol) + " " + tr("URL"));
		uu -> insert("url", QVariant(name));
		uu -> insert("protocol", QVariant(protocol));
		return karin::chm_engine::RemoteURL;
	}
	else if(LCHMUrlFactory::isJavascriptURL(name))
	{
		if(dbg)
			doDebug(karin::DebugOutput, tr("This is a JavaScript URL"));
		uu -> insert("url", QVariant(name));
		return karin::chm_engine::JavaScriptURL;
	}
	else if(LCHMUrlFactory::isNewChmURL(name, cpp_chmfile, new_chm, new_page))
	{
		if(dbg)
			doDebug(karin::DebugOutput, tr("This is a new chm file URL, and path should be") + " " + new_chm + ", " + tr("and requested page path is") + " " + new_page);
		uu -> insert("file", QVariant(new_chm));
		uu -> insert("page", QVariant(new_page));
		return karin::chm_engine::NewChmURL;
	}
	else
	{
		if(!name.isEmpty() && !name.startsWith('.') && !name.startsWith('/'))
		{
			if(dbg)
				doDebug(karin::DebugOutput, tr("This may be a remote URL"));
			uu -> insert("url", QVariant(QUrl::fromUserInput(name)));
		}
		else
			uu -> insert("url", QVariant(QUrl()));
		return karin::chm_engine::InternalURL_or_OthersURL;
	}
}

bool karin::chm_engine::hasFile(const QString &name)
{
	return LCHMFile::impl() -> hasFile(name);
}

