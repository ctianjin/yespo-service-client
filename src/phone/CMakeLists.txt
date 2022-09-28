# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
IF( WIN32 )
    SET( QT_USE_QTMAIN TRUE )
ENDIF( WIN32 )

SET( QT_USE_QTXML TRUE )

INCLUDE( ${QT_USE_FILE} )

# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
IF( BUILDSYS_PHONE_ENABLE_PREFERENCE_DIALOG )
    ADD_DEFINITIONS( -DBUILDSYS_PHONE_ENABLE_PREFERENCE_DIALOG )
ENDIF( BUILDSYS_PHONE_ENABLE_PREFERENCE_DIALOG )
IF( BUILDSYS_PHONE_ENABLE_TRANSLUCENT )
    ADD_DEFINITIONS( -DBUILDSYS_PHONE_ENABLE_TRANSLUCENT )
ENDIF( BUILDSYS_PHONE_ENABLE_TRANSLUCENT )

# For translucent workaround
IF( BUILDSYS_PHONE_ENABLE_TRANSLUCENT )
    IF( BUILDSYS_PHONE_ENABLE_TRANSLUCENT_WORKAROUND )
        ADD_DEFINITIONS( -DBUILDSYS_PHONE_ENABLE_TRANSLUCENT_WORKAROUND )
    ENDIF( BUILDSYS_PHONE_ENABLE_TRANSLUCENT_WORKAROUND )
ENDIF( BUILDSYS_PHONE_ENABLE_TRANSLUCENT )

IF( BUILDSYS_PHONE_DYNAMIC_SERVER )
	ADD_DEFINITIONS( -DBUILDSYS_PHONE_DYNAMIC_SERVER )
ENDIF( BUILDSYS_PHONE_DYNAMIC_SERVER )

# --------------------------------------------------------------------
# phone
# --------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    .
    ./settings
    ../service
    ../ui
    ../pref
    ../utility
)

BUILDSYS_AUTO_SOURCES(
    phone_SOURCES
    logindialog.cpp
	audionavigate.cpp
	phoneapplication.cpp
	phonemainwindow.cpp
	abstractphoneview.cpp
	phoneview.cpp
	phonehomeview.cpp
	phonemakecallview.cpp
	phoneaddcontactview.cpp
	phoneinterpreterview.cpp
	phonecallingview.cpp
	phonecontactview.cpp
	phonehistoryview.cpp
	callinterpreterhelp.cpp
	callcontrolpanel.cpp
	callsplash.cpp
	aboutdialog.cpp
	updatedialog.cpp
	contactsitems_p.cpp
	recentlistitems_p.cpp
	callinfo.cpp
	messagepanel.cpp
	interpretationselect.cpp
	popupdirectcall.cpp
	popupdtmf.cpp
	popupvolume.cpp
	contactprofile.cpp
	phoneglobal.cpp
	dtmfplate.cpp
	contactstreewidget.cpp
	historytreewidget.cpp
    main.cpp
    logindialog.ui
	audionavigate.ui
	phonemainwindow.ui
	phoneview.ui
	phonehomeview.ui
	phonemakecallview.ui
	phoneaddcontactview.ui
	phoneinterpreterview.ui
	phonecallingview.ui
	phonecontactview.ui
	phonehistoryview.ui
	callinterpreterhelp.ui
	callcontrolpanel.ui
	callsplash.ui
	aboutdialog.ui
	updatedialog.ui
	messagepanel.ui
	interpretationselect.ui
	popupdirectcall.ui
	popupdtmf.ui
	popupvolume.ui
	contactprofile.ui
	dtmfplate.ui
	settings/abstractsettingspage.cpp
    settings/settingsdialog.cpp
    settings/settingsgeneralpage.cpp
    settings/settingsdialog.ui
    settings/settingsgeneralpage.ui
	phone.qrc
	phone.rc.in
)

ADD_EXECUTABLE( phone WIN32 ${phone_SOURCES} )
ADD_DEPENDENCIES( phone translator up2date )

TARGET_LINK_LIBRARIES( phone service ui pref utility ${QT_LIBRARIES} )

SET_TARGET_PROPERTIES( phone PROPERTIES OUTPUT_NAME ${BUILDSYS_TELEPHONE_OUTPUT_NAME} )

INSTALL( TARGETS phone ${INSTALL_TARGETS_DEFAULT_ARGS} )
