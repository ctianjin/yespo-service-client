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

# --------------------------------------------------------------------
# phone
# --------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    .
#    ./settings
    ../service
    ../ui
    ../pref
    ../utility
)

BUILDSYS_AUTO_SOURCES(
    capability_SOURCES
    main.cpp
	capabilityapplication.cpp
	callerdialog.cpp
	calleedialog.cpp
	configinfo.cpp
	callerdialog.ui
	calleedialog.ui
	capability.qrc
	capability.rc.in
)

ADD_EXECUTABLE( capability WIN32 ${capability_SOURCES} )
# ADD_DEPENDENCIES( capability up2date )

TARGET_LINK_LIBRARIES( capability service ui pref utility ${QT_LIBRARIES} )

SET_TARGET_PROPERTIES( capability PROPERTIES OUTPUT_NAME ${BUILDSYS_CAPABILITY_OUTPUT_NAME} )

INSTALL( TARGETS capability ${INSTALL_TARGETS_DEFAULT_ARGS} )
