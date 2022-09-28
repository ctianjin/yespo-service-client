# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
INCLUDE( ${QT_USE_FILE} )


# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
IF( BUILDSYS_UI_USE_CLEANLOOKS )
    ADD_DEFINITIONS( -DBUILDSYS_UI_USE_CLEANLOOKS )
ENDIF( BUILDSYS_UI_USE_CLEANLOOKS )
IF( BUILDSYS_UI_USE_QTCURVE )
    ADD_DEFINITIONS( -DBUILDSYS_UI_USE_QTCURVE )
ENDIF( BUILDSYS_UI_USE_QTCURVE )
IF( BUILDSYS_OFFICAL)
	ADD_DEFINITIONS( -DBUILDSYS_OFFICAL)
ENDIF( BUILDSYS_OFFICAL)

# --------------------------------------------------------------------
# ui
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ../pref
    ../db
    ../utility
    ../intl
)

IF( BUILDSYS_ENABLE_SINGLE_APPLICATION )
    INCLUDE_DIRECTORIES(
        ${BUILDSYS_LIBS_DIR}/qtsingleapplication
        ${BUILDSYS_LIBS_DIR}/qtlockedfile
    )
ENDIF( BUILDSYS_ENABLE_SINGLE_APPLICATION)

IF( BUILDSYS_UI_USE_QTCURVE )
    INCLUDE_DIRECTORIES(
        ${BUILDSYS_LIBS_DIR}/qtcurve
        ${BUILDSYS_LIBS_DIR}/qtcurve/common
        ${BUILDSYS_LIBS_DIR}/qtcurve/style
    )
ENDIF( BUILDSYS_UI_USE_QTCURVE )

BUILDSYS_AUTO_LIBRARY_SOURCES(
    ui_SOURCES
    customloadinglabel.cpp
    customfadewidget.cpp
    custommoviewidget.cpp
    customfontlabel.cpp
    voicewidget.cpp
#    abstractactionedit.cpp
    application.cpp
    customtreewidget.cpp
    checkproxymodel.cpp
#    coloredit.cpp
    confirmbox.cpp
#    fontedit.cpp
    framelesshelper.cpp
    framelessmainwindow.cpp
    framelessdialog.cpp
    framelessvideodialog.cpp
    imagelabel.cpp
    mainwindow.cpp
    preferencedialog.cpp
	promptdialog.cpp
    taskexecutor.cpp
    uistyle.cpp
#	smartedit.cpp
#	searchedit.cpp
	popupbutton.cpp
	floatpanel.cpp
	simplecrypt.cpp
	elidedlabel.cpp
	boxshadow.cpp
	videolabel.cpp
    preferencedialog.ui
	promptdialog.ui
    ui.qrc
    ui.rc.in
)

ADD_LIBRARY( ui ${ui_SOURCES} )
TARGET_LINK_LIBRARIES( ui pref db utility intl ${QT_LIBRARIES} )
IF( BUILDSYS_ENABLE_SINGLE_APPLICATION )
    TARGET_LINK_LIBRARIES( ui qtsingleapplication )
ENDIF( BUILDSYS_ENABLE_SINGLE_APPLICATION)
IF( BUILDSYS_UI_USE_QTCURVE )
    TARGET_LINK_LIBRARIES( ui qtcurve )
ENDIF( BUILDSYS_UI_USE_QTCURVE )
SET_TARGET_PROPERTIES( ui PROPERTIES OUTPUT_NAME ${BUILDSYS_UI_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_UI_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS ui ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
