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

IF( BUILDSYS_DEMO )
	ADD_DEFINITIONS( -DBUILDSYS_DEMO )
ENDIF( BUILDSYS_DEMO )

IF( BUILDSYS_LOCAL )
	ADD_DEFINITIONS( -DBUILDSYS_LOCAL )
ENDIF( BUILDSYS_LOCAL )

IF( BUILDSYS_OFFICAL)
	ADD_DEFINITIONS( -DBUILDSYS_OFFICAL)
ENDIF( BUILDSYS_OFFICAL)

# --------------------------------------------------------------------
# translator
# --------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    .
	./settings
	../proto
    ../service
	../im
    ../ui
    ../pref
    ../utility
	${QTMOBILITY_INCLUDE_DIRS}
	${QT_QTWEBKIT_INCLUDE_DIR}
	${CRASHHANDLER_INCLUDE_DIR}
	${SMTP_INCLUDE_DIR}
)

BUILDSYS_AUTO_SOURCES(
    translator_SOURCES
	audionavigate.cpp
	browser.cpp
	phoneapplication.cpp
	phonemainwindow.cpp
	callsplash.cpp
	callmode.cpp
	closemethoddialog.cpp
	aboutdialog.cpp
	imageview.cpp
	updatedialog.cpp
	recentlistitems_p.cpp
	tonegoglobal.cpp
	messagepanel.cpp
	pagewelcome.cpp
	pagelogin.cpp
	pageframewindow.cpp
	pagecalling.cpp
    remotevideodialog.cpp
	remotivideosetting.cpp
	reportnotify.cpp
	skillchannel.cpp
    sipnotify.cpp
	sippreferencedialog.cpp
	stun.cpp
	viewidle.cpp
	translationlanguage.cpp	
	viewrecentdetail.cpp
	viewcontact.cpp
	viewprofile.cpp
	viewincomingcall.cpp
	udp.cpp
	testnetwork.cpp
	testnetworkcalculate.cpp
	testdevicestart.cpp
	testvoice.cpp
	testcamera.cpp
	testdevicefinished.cpp
    widgetphototranslation.cpp
	protocolerror.cpp
    main.cpp
	audionavigate.ui
	browser.ui
	phonemainwindow.ui
	callmode.ui
	callsplash.ui
	closemethoddialog.ui
	aboutdialog.ui
	imageview.ui
	updatedialog.ui
	messagepanel.ui
	pagewelcome.ui
	pageframewindow.ui
	pagelogin.ui
	pagecalling.ui
	remotevideodialog.ui
	remotivideosetting.ui
	reportnotify.ui
	skillchannel.ui
    sipnotify.ui
	sippreferencedialog.ui
	viewidle.ui
	translationlanguage.ui
	viewrecentdetail.ui
	viewcontact.ui
	viewprofile.ui
	viewincomingcall.ui
	testnetwork.ui
	testnetworkcalculate.ui
	testdevicestart.ui
	testvoice.ui
	testcamera.ui
	testdevicefinished.ui
	widgetphototranslation.ui
	settings/abstractsettingspage.cpp
    settings/settingsdialog.cpp
    settings/settingsgeneralpage.cpp
	settings/settingslanguagepage.cpp
	settings/settingshintpage.cpp
	settings/settingscallinpage.cpp
	settings/settingsblacklistpage.cpp
	settings/settingsupdatepage.cpp
    settings/settingsdialog.ui
    settings/settingsgeneralpage.ui
	settings/settingslanguagepage.ui
	settings/settingshintpage.ui
	settings/settingscallinpage.ui
	settings/settingsblacklistpage.ui
	settings/settingsupdatepage.ui
	translator.qrc
	translator.rc.in
)

ADD_EXECUTABLE( translator WIN32 ${translator_SOURCES} )
ADD_DEPENDENCIES( translator capability up2date reporter )

TARGET_LINK_LIBRARIES( translator service ui pref im utility proto ${QT_LIBRARIES} ${QTMOBILITY_LIBRARIES} ${QT_QTWEBKIT_LIBRARY} ${CRASHHANDLER_LIBRARIES} ${SMTP_LIBRARIES} ) 

SET_TARGET_PROPERTIES( translator PROPERTIES OUTPUT_NAME ${BUILDSYS_TRANSLATOR_OUTPUT_NAME} )

INSTALL( TARGETS translator ${INSTALL_TARGETS_DEFAULT_ARGS} )
