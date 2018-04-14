#ifndef _KARIN_STD_H
#define _KARIN_STD_H

#include <QFont>
#include <cmath>
#include <cstdlib>
class QString;

namespace karin
{
	enum OutputType
	{
		DebugOutput = 0,
		WarningOutput,
		FatalOutput,
		NoOutput
	};
	enum SizeType
	{
		Tiny = 0,
		VerySmall,
		Small,
		Normal,
		Large,
		VeryLarge,
		Super,
		FullSize
	};
	enum OperationResult
	{
		Failed = 0,
		Nothing,
		Successful
	};

	QString decodeUrl(const QString &url);
	QString getOutputTypeString(OutputType type);
	QString getOutputTypeIconId(OutputType type);
	const QFont & getConstantFont(SizeType t = Normal, QFont::Weight weight = QFont::Normal);
	QString getRemoteUrlName(const QString &protocol);
	inline double ComputeAngle(int x, int y)
	{
		double a = (double)x;
		double b = (double)y;
		if(a == 0 && b > 0)
			return 270.0;
		if(a == 0 && b < 0)
			return 90.0;
		if(b == 0 && a > 0)
			return 0.0;
		if(b == 0 && a < 0)
			return 180.0;
		double c = sqrt(a * a + b * b);
		//bxb+bxb/2bc
		double arg = acos((double)(abs(a)) / c) * (180.0 / M_PI);
		if(a > 0 && b > 0)
		{
			return 360.0 - arg;
		}
		else if(a > 0 && b < 0)
		{
			return arg;
		}
		else if(a < 0 && b > 0)
		{
			return 180.0 + arg;
		}
		else if(a < 0 && b < 0)
		{
			return 180.0 - arg;
		}
		else
			return 0.0;
	}
}

#endif
