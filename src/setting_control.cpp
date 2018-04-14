#include "setting_control.h"
#include "button_with_label.h"
#include "karin_ut.h"
#include "header_view.h"
#include "karin_std.h"

#include <MButton>
#include <MButtonGroup>
#include <MContainer>
#include <MLayout>
#include <MSlider>
#include <MTextEdit>
#include <MLabel>
#include <MLinearLayoutPolicy>
#include <QIntValidator>
#include <MPannableViewport>
#include <QGraphicsAnchorLayout>

#include <climits>

karin::setting_control::setting_control(QGraphicsItem *parent)
	:MApplicationPage(parent),
	oriGroup(new MButtonGroup(this)),
	fullscreen(new karin::button_with_label(karin::button_with_label::switchType)),
	show_splash(new karin::button_with_label(karin::button_with_label::switchType)),
	resultCount(new MTextEdit),
	zoomRange(new MSlider),
	ignoreAngleRange(new MSlider),
	doubleclick_zoom(new karin::button_with_label(karin::button_with_label::switchType)),
	doubleClickZoomRange(new MSlider),
	extUrl(new karin::button_with_label),
	extChm(new karin::button_with_label)
{
	karin::header_view *header = new karin::header_view;
	MPannableViewport *pan = new MPannableViewport;
	MWidget *widget = new MWidget;
	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout;
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	MLayout *mainLayout = new MLayout;
	MLinearLayoutPolicy *mainPolicy = new MLinearLayoutPolicy(mainLayout, Qt::Vertical);
	MContainer *oriContainer = new MContainer;
	MLayout *windowLayout = new MLayout;
	MLinearLayoutPolicy *windowPolicy = new MLinearLayoutPolicy(windowLayout, Qt::Vertical);
	MLayout *oriLayout = new MLayout;
	MLinearLayoutPolicy *oriPolicy = new MLinearLayoutPolicy(oriLayout, Qt::Vertical);
	MButton *autoO = new MButton(tr("Automatic Orientation"));
	MButton *portO = new MButton(tr("Lock Portrait"));
	MButton *landO = new MButton(tr("Lock Landscape"));
	MContainer *viewContainer = new MContainer;
	MLayout *viewLayout = new MLayout;
	viewPolicy = new MLinearLayoutPolicy(viewLayout, Qt::Vertical);
	MLabel *resultCountLabel = new MLabel(tr("Search query result max count"));
	MLabel *zoomRangeLabel = new MLabel(tr("Zoom range") + " (1x - 32x)");
	MLabel *ignoreAngleRangeLabel = new MLabel(tr("Ignore slide angle range") + " (0° - 35°)");
	doubleClickZoomRangeLabel = new MLabel(tr("Double-Click zoom range") + " (4x - 16x)");
	MContainer *handleContainer = new MContainer;
	MLayout *handleLayout = new MLayout;
	MLinearLayoutPolicy *handlePolicy = new MLinearLayoutPolicy(handleLayout, Qt::Vertical);
	QIntValidator *intValidator = new QIntValidator(this);
	setPannable(false);
	setTitle(tr("Settings") + " - Karin CHM Viewer");
	header -> setTitleFont(karin::getConstantFont(karin::VeryLarge));
	header -> setTitle(tr("Setting"));
	oriPolicy ->setNotifyWidgetsOfLayoutPositionEnabled(true);
	autoO -> setCheckable(true);
	portO -> setCheckable(true);
	landO -> setCheckable(true);
	autoO -> setViewType(MButton::groupType);
	portO -> setViewType(MButton::groupType);
	landO -> setViewType(MButton::groupType);
	oriGroup -> addButton(autoO, 0);
	oriGroup -> addButton(portO, 1);
	oriGroup -> addButton(landO, 2);
	fullscreen -> setTitle(tr("Full Screen"));
	show_splash -> setTitle(tr("Show Splash"));
	oriPolicy -> addItem(autoO);
	oriPolicy -> addItem(portO);
	oriPolicy -> addItem(landO);
	windowPolicy -> addItem(oriLayout);
	windowPolicy -> addItem(fullscreen);
	windowPolicy -> addItem(show_splash);
	oriContainer -> setTitle(tr("Window"));
	oriContainer -> centralWidget() -> setLayout(windowLayout);
	connect(fullscreen, SIGNAL(clicked(bool)), this, SIGNAL(setFullscreen(bool)));
	connect(show_splash, SIGNAL(clicked(bool)), this, SLOT(setShowSplash(bool)));
	zoomRange -> setRange(1, 32);
	doubleClickZoomRange -> setRange(4, 16);
	ignoreAngleRange -> setRange(0, 35);
	zoomRange -> setHandleLabelVisible(true);
	doubleClickZoomRange -> setHandleLabelVisible(true);
	ignoreAngleRange -> setHandleLabelVisible(true);
	intValidator -> setRange(1, INT_MAX);
	resultCount -> setValidator(intValidator);
	resultCount -> setContentType(M::NumberContentType);
	resultCount -> setPrompt(QString("%1 - %2").arg(1).arg(INT_MAX));
	resultCount -> setErrorHighlight(true);
	doubleclick_zoom -> setTitle(tr("Double-click to zoom"));
	viewPolicy -> addItem(resultCountLabel);
	viewPolicy -> addItem(resultCount);
	viewPolicy -> addItem(zoomRangeLabel);
	viewPolicy -> addItem(zoomRange);
	viewPolicy -> addItem(ignoreAngleRangeLabel);
	viewPolicy -> addItem(ignoreAngleRange);
	viewPolicy -> addItem(doubleclick_zoom);
	viewContainer -> setTitle(tr("Viewer"));
	connect(zoomRange, SIGNAL(valueChanged(int)), this, SLOT(setZoomRange(int)));
	connect(doubleClickZoomRange, SIGNAL(valueChanged(int)), this, SLOT(setDoubleClickZoomRange(int)));
	connect(resultCount, SIGNAL(lostFocus(Qt::FocusReason)), this, SLOT(setSearchResultCount()));
	connect(ignoreAngleRange, SIGNAL(valueChanged(int)), this, SLOT(setIgnoreAngleRange(int)));
	connect(resultCount, SIGNAL(returnPressed()), this, SLOT(setSearchResultCount()));
	connect(doubleclick_zoom, SIGNAL(clicked(bool)), this, SLOT(setDoubleClickZoom(bool)));
	//connect(resultCount, SIGNAL(textChanged()), this, SLOT(setSearchResultCount()));
	viewContainer -> centralWidget() -> setLayout(viewLayout);
	extUrl -> setTitle(tr("Handle remote URL request"));
	extChm -> setTitle(tr("Handle new chm file request"));
	connect(extUrl, SIGNAL(clicked(bool)), this, SLOT(doHandleExternalUrl(bool)));
	connect(extChm, SIGNAL(clicked(bool)), this, SLOT(doHandleNewChm(bool)));
	handlePolicy -> addItem(extUrl);
	handlePolicy -> addItem(extChm);
	handleContainer -> setTitle(tr("Link Handler"));
	handleContainer -> centralWidget() -> setLayout(handleLayout);
	connect(oriGroup, SIGNAL(buttonClicked(int)), this, SIGNAL(changeOrientation(int)));
	mainPolicy -> addItem(oriContainer);
	mainPolicy -> addItem(viewContainer);
	mainPolicy -> addItem(handleContainer);
	widget -> setLayout(mainLayout);
	pan -> setWidget(widget);
	layout -> addAnchor(header, Qt::AnchorTop, layout, Qt::AnchorTop);
	layout -> addAnchor(header, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout -> addAnchor(header, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout -> addAnchor(pan, Qt::AnchorTop, header, Qt::AnchorBottom);
	layout -> addAnchor(pan, Qt::AnchorBottom, layout, Qt::AnchorBottom);
	layout -> addAnchor(pan, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout -> addAnchor(pan, Qt::AnchorRight, layout, Qt::AnchorRight);
	centralWidget() -> setLayout(layout);
}

karin::setting_control::~setting_control()
{
}

void karin::setting_control::init()
{
	int id = karin::ut::Instance() -> getSetting<int>(ORIENTATION);
	oriGroup -> button(id) -> setChecked(true);
	fullscreen -> setChecked(karin::ut::Instance() -> getSetting<bool>(FULL_SCREEN));
	show_splash -> setChecked(karin::ut::Instance() -> getSetting<bool>(SHOW_SPLASH));
	extUrl -> setChecked(karin::ut::Instance() -> getSetting<bool>(HANDLE_EXTERNAL_URL));
	extChm -> setChecked(karin::ut::Instance() -> getSetting<bool>(HANDLE_NEW_CHM));
	zoomRange -> setValue(karin::ut::Instance() -> getSetting<int>(ZOOM_RANGE));
	doubleClickZoomRange -> setValue(karin::ut::Instance() -> getSetting<int>(DOUBLE_CLICK_ZOOM_RANGE));
	ignoreAngleRange -> setValue(karin::ut::Instance() -> getSetting<int>(SLIDE_IGNORE_ANGLE));
	resultCount -> setText(QString::number(karin::ut::Instance() -> getSetting<unsigned int>(SEARCH_RESULT_COUNT)));
	bool zb = karin::ut::Instance() -> getSetting<bool>(DOUBLE_CLICK_ZOOM);
	doubleclick_zoom -> setChecked(zb);
	if(zb)
	{
		viewPolicy -> addItem(doubleClickZoomRangeLabel);
		viewPolicy -> addItem(doubleClickZoomRange);
	}
}

void karin::setting_control::doHandleExternalUrl(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(HANDLE_EXTERNAL_URL, b);
}

void karin::setting_control::doHandleNewChm(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(HANDLE_NEW_CHM, b);
}

void karin::setting_control::setZoomRange(int value)
{
	if(value > 0 && value <= 32)
	{
		zoomRange -> setHandleLabel(QString("%1x").arg(value));
		karin::ut::Instance() -> setSetting<int>(ZOOM_RANGE, value);
	}
}

void karin::setting_control::setIgnoreAngleRange(int value)
{
	if(value >= 0 && value <= 40)
	{
		ignoreAngleRange -> setHandleLabel(QString("%1°").arg(value));
		karin::ut::Instance() -> setSetting<int>(SLIDE_IGNORE_ANGLE, value);
	}
}

void karin::setting_control::setSearchResultCount()
{
	bool b = false;
	int i = 0;
	QString s = resultCount -> text();
	if(s.isEmpty())
	{
		i = 100;
		resultCount -> setText(QString::number(i));
	}
	else
	{
		i = s.toInt(&b);
		if(!b)
		{
			i = 100;
			resultCount -> setText(QString::number(i));
		}
		if(i < 1)
		{
			i = 1;
			resultCount -> setText(QString::number(i));
		}
		else if(i > INT_MAX)
		{
			i = INT_MAX;
			resultCount -> setText(QString::number(i));
		}
	}
	karin::ut::Instance() -> setSetting<unsigned int>(SEARCH_RESULT_COUNT, i);
}

void karin::setting_control::setShowSplash(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(SHOW_SPLASH, b);
}

void karin::setting_control::setDoubleClickZoom(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(DOUBLE_CLICK_ZOOM, b);
	if(b)
	{
		if(viewPolicy -> indexOf(doubleClickZoomRangeLabel) == -1)
			viewPolicy -> addItem(doubleClickZoomRangeLabel);
		if(viewPolicy -> indexOf(doubleClickZoomRange) == -1)
			viewPolicy -> addItem(doubleClickZoomRange);
	}
	else
	{
		if(viewPolicy -> indexOf(doubleClickZoomRangeLabel) != -1)
			viewPolicy -> removeItem(doubleClickZoomRangeLabel);
		if(viewPolicy -> indexOf(doubleClickZoomRange) != -1)
			viewPolicy -> removeItem(doubleClickZoomRange);
	}
}

void karin::setting_control::setDoubleClickZoomRange(int value)
{
	if(value >= 4 && value <= 16)
	{
		doubleClickZoomRange -> setHandleLabel(QString("%1x").arg(value));
		karin::ut::Instance() -> setSetting<int>(DOUBLE_CLICK_ZOOM_RANGE, value);
	}
}

