#include "about_dialog.h"
#include "karin_std.h"

#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <QStringList>
#include <QDesktopServices>
#include <QUrl>

karin::about_dialog::about_dialog()
	:MDialog(),
	contentText(new MLabel),
	contentImage(new MImageWidget)
{
	MLayout *layout = new MLayout;
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	addButton(M::OkButton);
	setCloseButtonVisible(true);
	contentText -> setFont(karin::getConstantFont());
	contentText -> setWrapMode(QTextOption::WordWrap);
	contentText -> setWordWrap(true);
	contentText -> setColor(QColor("#FFFFFF"));
	contentText -> setAlignment(Qt::AlignCenter);
	policy -> addItem(contentImage);
	policy -> addItem(contentText);
	connect(contentText, SIGNAL(linkActivated(const QString &)), this, SLOT(openLink(const QString &)));
	centralWidget() -> setLayout(layout);
}

karin::about_dialog::~about_dialog()
{
}

void karin::about_dialog::setImagePath(const QString &path)
{
	contentImage -> setImage(QImage(path));
}

void karin::about_dialog::setContentTexts(const QStringList &texts)
{
	contentText -> setText(texts.join("<br>"));
}

void karin::about_dialog::openLink(const QString &link)
{
	QDesktopServices::openUrl(QUrl(link));
}
