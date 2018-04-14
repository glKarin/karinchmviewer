#ifndef _KARIN_SETTINGCONTROL_H
#define _KARIN_SETTINGCONTROL_H

#include <MApplicationPage>

class MButtonGroup;
class MTextEdit;
class MSlider;
class MLinearLayoutPolicy;
class MLabel;

namespace karin
{
	class header_view;
	class button_with_label;

	class setting_control : public MApplicationPage
	{
		Q_OBJECT
			
		public:
			setting_control(QGraphicsItem *parent = 0);
			virtual ~setting_control();
			void init();

			private Q_SLOTS:
				void doHandleExternalUrl(bool b);
				void doHandleNewChm(bool b);
				void setZoomRange(int value);
				void setIgnoreAngleRange(int value);
				void setSearchResultCount();
				void setShowSplash(bool b);
				void setDoubleClickZoom(bool b);
				void setDoubleClickZoomRange(int value);

Q_SIGNALS:
				void changeOrientation(int id);
				void setFullscreen(bool b);

		private:
				MButtonGroup *oriGroup;
				button_with_label *fullscreen;
				button_with_label *show_splash;
				MTextEdit *resultCount;
				MSlider *zoomRange;
				MSlider *ignoreAngleRange;
				button_with_label *doubleclick_zoom;
				MSlider *doubleClickZoomRange;
				button_with_label *extUrl;
				button_with_label *extChm;
				MLinearLayoutPolicy *viewPolicy;
				MLabel *doubleClickZoomRangeLabel;

		private:
			setting_control(const setting_control &other);
			setting_control & operator= (const setting_control &other);
	};
}

#endif
