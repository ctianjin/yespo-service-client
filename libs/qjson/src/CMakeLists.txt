# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_DONT_USE_QTGUI TRUE )
IF( USE_QT5 )
	INCLUDE_DIRECTORIES( ${Qt5Core_INCLUDE_DIRS} )
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
# qjson
# --------------------------------------------------------------------
SET( QJSON_LIB_MAJOR_VERSION "0" )
SET( QJSON_LIB_MINOR_VERSION "7" )
SET( QJSON_LIB_PATCH_VERSION "1" )
SET( QJSON_LIB_VERSION_STRING "${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}" )

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
)

SET(
    qjson_SOURCES
    parser.cpp
    qobjecthelper.cpp
    json_scanner.cpp
    json_parser.cc
    parserrunnable.cpp
    serializer.cpp
    serializerrunnable.cpp
)

IF( USE_QT5 )
	QT5_WRAP_CPP(
		qjson_SOURCES
		parserrunnable.h
		serializerrunnable.h
	)
ELSE()
	QT4_WRAP_CPP(
		qjson_SOURCES
		parserrunnable.h
		serializerrunnable.h
	)
ENDIF()

ADD_LIBRARY( qjson STATIC ${qjson_SOURCES} )
IF( USE_QT5 )
	QT5_USE_MODULES( qjson Core )
ELSE()
	TARGET_LINK_LIBRARIES( qjson ${QT_LIBRARIES} )
ENDIF()
SET_TARGET_PROPERTIES( qjson PROPERTIES
    AUTOMOC FALSE
    VERSION ${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}
    SOVERSION ${QJSON_LIB_MAJOR_VERSION}
    DEFINE_SYMBOL QJSON_MAKEDLL
)
