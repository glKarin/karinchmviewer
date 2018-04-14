#include "karin_std.h"
#include <QString>
#include <QChar>
#include <QObject>

QString karin::decodeUrl(const QString &input)
{
	QString temp;
	int len = input.length();
	for( int i = 0 ; i < len; ++i )
	{
		QChar c = input[i];
		if (c == '%' && i + 2 < len)
		{
			int a = input[++i].unicode();
			int b = input[++i].unicode();

			if (a >= '0' && a <= '9')
				a -= '0';
			else if (a >= 'a' && a <= 'f')
				a = a - 'a' + 10;
			else if (a >= 'A' && a <= 'F')
				a = a - 'A' + 10;

			if (b >= '0' && b <= '9')
				b -= '0';
			else if (b >= 'a' && b <= 'f')
				b  = b - 'a' + 10;
			else if (b >= 'A' && b <= 'F')
				b  = b - 'A' + 10;

			temp.append( (QChar)((a << 4) | b ) );
		}
		else
		{
			temp.append( c );
		}
	}
	return temp;
}

QString karin::getOutputTypeString(karin::OutputType type)
{
	switch(type)
	{
		case karin::DebugOutput:
			return QObject::tr("INFO");
		case karin::WarningOutput:
			return QObject::tr("WARNING");
		case karin::FatalOutput:
			return QObject::tr("ERROR");
		default:
			break;
	}
	return "";
}

QString karin::getOutputTypeIconId(karin::OutputType type)
{
	static const QString iconIds[karin::NoOutput] = 
	{
		"icon-l-terminal",
		"icon-l-user-guide",
		"icon-l-error"
	};
	return iconIds[type];
}

const QFont & karin::getConstantFont(karin::SizeType t, QFont::Weight w)
{
	static QFont font;
	static const int pixelSizes[karin::FullSize] =
	{
		10,
		15,
		20,
		25,
		30,
		35,
		40
	};
	font.setWeight(w);
	font.setPixelSize(pixelSizes[t]);
	return font;
}

/*
QString karin::getIndentSpacing(int indent)
{
	QString s;
	for(int i = 0; i < indent; i++)
		s += "    ";
	return s;
}
*/

QString karin::getRemoteUrlName(const QString &protocol)
{
	if(protocol == "http")
		return QObject::tr("Http");
	else if(protocol == "ftp")
		return QObject::tr("FTP");
	else if(protocol == "mailto")
		return QObject::tr("Mail");
	else if(protocol == "news")
		return QObject::tr("News");
	else
		return QObject::tr("Other Type");
}
