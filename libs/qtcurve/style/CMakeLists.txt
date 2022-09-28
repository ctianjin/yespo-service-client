# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
IF( Q_WS_X11 )
    SET( QT_USE_QTDBUS TRUE )
ENDIF( Q_WS_X11 )
SET( QT_USE_QTSVG TRUE )

IF( USE_QT5 )
	INCLUDE_DIRECTORIES( ${Qt5Widgets_INCLUDE_DIRS} )
	INCLUDE_DIRECTORIES( ${Qt5Svg_INCLUDES_DIRS} )
ELSE()
	INCLUDE( ${QT_USE_FILE} )
ENDIF()

# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
BUILDSYS_ENABLE_EXCEPTIONS( FALSE )
BUILDSYS_ENABLE_RTTI( FALSE )

# --------------------------------------------------------------------
# qtcurve
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ../common
)

SET(
    qtcurve_style_common_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/config_file.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/colorutils.c
)
SET_SOURCE_FILES_PROPERTIES( ${qtcurve_style_common_SRCS} PROPERTIES LANGUAGE CXX )

IF( Q_WS_X11 )
    SET( qtcurve_SRCS qtcurve.cpp windowmanager.cpp macmenu.cpp blurhelper.cpp utils.cpp shadowhelper.cpp shortcuthandler.cpp ${qtcurve_style_common_SRCS} )
    SET( qtcurve_MOC_HDRS qtcurve.h windowmanager.h macmenu.h macmenu-dbus.h blurhelper.h shadowhelper.h shortcuthandler.h )
ELSE( Q_WS_X11 )
    SET( qtcurve_SRCS qtcurve.cpp windowmanager.cpp blurhelper.cpp utils.cpp shortcuthandler.cpp ${qtcurve_style_common_SRCS} )
    SET( qtcurve_MOC_HDRS qtcurve.h windowmanager.h blurhelper.h shortcuthandler.h )
ENDIF( Q_WS_X11 )

IF( USE_QT5 )
#	QT5_AUTOMOC( ${qtcurve_SRCS} )
	QT5_WRAP_CPP( qtcurve_MOC_SRCS ${qtcurve_MOC_HDRS} )
ELSE()
	QT4_AUTOMOC( ${qtcurve_SRCS} )
	QT4_WRAP_CPP( qtcurve_MOC_SRCS ${qtcurve_MOC_HDRS} )
ENDIF()

ADD_LIBRARY( qtcurve STATIC ${qtcurve_SRCS} ${qtcurve_MOC_SRCS} )
SET_TARGET_PROPERTIES( qtcurve PROPERTIES AUTOMOC FALSE )

IF( Q_WS_X11 )
    TARGET_LINK_LIBRARIES( qtcurve ${QT_LIBRARIES} ${X11_X11_LIB} )
ELSE( Q_WS_X11 )
	IF( USE_QT5 )
		QT5_USE_MODULES( qtcurve Widgets Svg )
	ELSE()
		TARGET_LINK_LIBRARIES( qtcurve ${QT_LIBRARIES} )
	ENDIF()
ENDIF( Q_WS_X11 )
