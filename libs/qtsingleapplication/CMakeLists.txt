
# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_USE_QTNETWORK TRUE )
IF( USE_QT5 )
	INCLUDE_DIRECTORIES( ${Qt5Widgets_INCLUDE_DIRS} )
	INCLUDE_DIRECTORIES( ${Qt5Network_INCLUDE_DIRS} )
ELSE()
	INCLUDE( ${QT_USE_FILE} )
ENDIF()

# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
BUILDSYS_ENABLE_EXCEPTIONS( FALSE )
BUILDSYS_ENABLE_RTTI( FALSE )

IF( UNIX )
    ADD_DEFINITIONS( -fPIC )
ENDIF( UNIX )

# --------------------------------------------------------------------
# qtsingleapplication
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ../qtlockedfile
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET(
    qtsingleapplication_SOURCES
    qtlocalpeer.cpp
    qtsinglecoreapplication.cpp
    qtsingleapplication.cpp
)

IF( USE_QT5 )
	QT5_WRAP_CPP(
		qtsingleapplication_SOURCES
		qtlocalpeer.h
		qtsinglecoreapplication.h
		qtsingleapplication.h
	)
ELSE()
	QT4_WRAP_CPP(
		qtsingleapplication_SOURCES
		qtlocalpeer.h
		qtsinglecoreapplication.h
		qtsingleapplication.h
	)

ENDIF()

ADD_LIBRARY( qtsingleapplication STATIC ${qtsingleapplication_SOURCES} )
IF( USE_QT5 )
	QT5_USE_MODULES( qtsingleapplication Widgets Network )
ELSE()
	TARGET_LINK_LIBRARIES( qtsingleapplication qtlockedfile ${QT_LIBRARIES} )
ENDIF()
SET_TARGET_PROPERTIES( qtsingleapplication PROPERTIES AUTOMOC FALSE )
