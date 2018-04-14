#ifndef _KARIN_BUTTONWITHLABEL_H
#define _KARIN_BUTTONWITHLABEL_H

#include <MWidgetController>

class MButton;
class MLabel;
class QFont;

namespace karin
{
	class button_with_label : public MWidgetController
	{
		Q_OBJECT

		public:
			enum karin_ViewType
			{
				checkboxType = 0,
				switchType
			};

			button_with_label(karin_ViewType type = checkboxType, QGraphicsItem *parent = 0);
			virtual ~button_with_label();
			void setTitle(const QString &title);
			void setTitleColor(const QString &color);
			void setTitleFont(const QFont &font);
			void setCheckable(bool b);
			void setChecked(bool b);
			void setEnabled(bool b);
			inline MButton * getButton()
			{
				return button;
			}

Q_SIGNALS:
			void pressed();
			void released();
			void clicked(bool checked = false);
			void toggled(bool checked);

		private:
			MLabel *label;
			MButton *button;

		private:
			button_with_label(const button_with_label &other);
			button_with_label & operator= (const button_with_label &other);
	};
}

#endif
