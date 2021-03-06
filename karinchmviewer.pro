######################################################################
# Automatically generated by qmake (2.01a) ?? 10? 23 07:24:01 2014
######################################################################

TEMPLATE = subdirs
SUBDIRS += lib src

contains(MEEGO_EDITION,harmattan){
splash.files = splash/karinchmviewer_splash.jpg
splash.path = /usr/share/karinchmviewer/splash

translation.files = i18n/karinchmviewer.zh_CN.ts i18n/karinchmviewer.zh_CN.qm
translation.path = /usr/share/karinchmviewer/i18n

mime.files = mime/karin_chm.xml
mime.path = /usr/share/mime/packages

icon.files = karinchmviewer_80.png
icon.path = /usr/share/karinchmviewer/icon

rsc.files = resources/karin-l-about.png resources/karin-l-collection.png resources/karin-l-document.png resources/karin-l-folder.png resources/karin-l-page.png resources/karin-l-setting.png resources/karin-m-directory.png resources/karin-m-file.png resources/kchmviewer.png resources/karin-l-quit.png 
rsc.path = /usr/share/karinchmviewer/resources

desktop.files = karinchmviewer.desktop
desktop.path = /usr/share/applications

css.files = style/karin_mcontentitemstyle.css
css.path = /usr/share/karinchmviewer/style


toolbar_icon.files = icons/icon-m-toolbar-karin-remove.png icons/icon-m-toolbar-karin-slide.png
toolbar_icon.path = /usr/share/themes/blanco/meegotouch/icons

INSTALLS += splash translation icon mime rsc desktop css 

}

