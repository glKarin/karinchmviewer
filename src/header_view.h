#ifndef _KARIN_HEADERVIEW_H
#define _KARIN_HEADERVIEW_H

#include <MWidgetController>

class MLabel;
class QColor;
class QFont;

namespace karin
{
	class header_view : public MWidgetController
	{
		public:
			enum karin_ViewType
			{
				headerType = 0,
				footerType
			};

			header_view(karin_ViewType type = headerType, QGraphicsItem *parent = 0);
			virtual ~header_view();
			void setTitle(const QString &title);
			void setTitleColor(const QString &color);
			void setTitleFont(const QFont &font);

		private:
			MLabel *label;

		private:
			header_view(const header_view &other);
			header_view & operator= (const header_view &other);
	};
}

#endif
