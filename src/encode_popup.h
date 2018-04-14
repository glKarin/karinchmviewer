#ifndef _KARIN_ENCODEPOPUP_H
#define _KARIN_ENCODEPOPUP_H

#include <MPopupList>

class QStringListModel;
class QModelIndex;

namespace karin
{
	class encode_popup : public MPopupList
	{
		Q_OBJECT

		public:
			encode_popup();
			virtual ~encode_popup();
			void reset(const QString &title);

Q_SIGNALS:
			void chooseEncode(const QString &e);

			private Q_SLOTS:
				void choose(const QModelIndex &index);

		private:
			QStringListModel *model;
			QStringList codecs;

		private:
			encode_popup(const encode_popup &other);
			encode_popup & operator= (const encode_popup &other);
	};
}

#endif
