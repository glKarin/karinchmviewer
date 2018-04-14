#include "encode_popup.h"
#include "chm_engine.h"

#include <QStringListModel>
#include <QVector>

karin::encode_popup::encode_popup()
	:MPopupList(),
	model(new QStringListModel(this))
{
	QStringList sl;
	QVector<karin::encoding_item> list = karin::chm_engine::getEncodingList();
	for(QVector<karin::encoding_item>::const_iterator itor = list.begin();
			itor != list.end();
			++itor)
	{
		sl.push_back(itor -> codec + " - " + itor -> family);
		codecs.push_back(itor -> codec);
	}
	model -> setStringList(sl);
	setItemModel(model);
	connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(choose(const QModelIndex &)));
}

karin::encode_popup::~encode_popup()
{
}

void karin::encode_popup::choose(const QModelIndex &index)
{
	int i = index.row();
	if(i >= codecs.size())
		return;
	emit chooseEncode(codecs.at(i));
}

void karin::encode_popup::reset(const QString &title)
{
	setTitle(tr("Current") + ": " + title);
	QModelIndex index;
	for(int i = 0; i < codecs.size(); i++)
		if(codecs.at(i) == title)
			index = model -> index(i);
	setCurrentIndex(index);
	scrollTo(index);
}
