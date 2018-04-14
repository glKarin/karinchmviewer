#ifndef _KARIN_ABOUTDIALOG_H
#define _KARIN_ABOUTDIALOG_H

#include <MDialog>

class MLabel;
class MImageWidget;
class QStringList;

namespace karin
{
	class about_dialog : public MDialog
	{
		Q_OBJECT

		public:
			about_dialog();
			virtual ~about_dialog();
			void setImagePath(const QString &path);
			void setContentTexts(const QStringList &texts);

		private Q_SLOTS:
			void openLink(const QString &link);

		private:
			MLabel *contentText;
			MImageWidget *contentImage;

		private:
			about_dialog(const about_dialog &other);
			about_dialog & operator= (const about_dialog &other);
	};
}

#endif
