# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
SET( BUILDSYS_TRANSLATIONS_LANGUAGES en_US zh_CN )
SET( BUILDSYS_TRANSLATIONS_LANGUAGES_QT zh_CN )

IF( WIN32 OR APPLE )
    SET( BUILDSYS_TRANSLATIONS_INCLUDE_QT FALSE )
ENDIF( WIN32 OR APPLE )

IF( BUILDSYS_TRANSLATIONS_INCLUDE_QT )
    CONFIGURE_FILE( intl_qt.qrc.in intl.qrc )
ELSE( BUILDSYS_TRANSLATIONS_INCLUDE_QT )
    CONFIGURE_FILE( intl.qrc.in intl.qrc )
ENDIF( BUILDSYS_TRANSLATIONS_INCLUDE_QT )

# --------------------------------------------------------------------
# translations
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( translations )

# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_DONT_USE_QTGUI TRUE )

INCLUDE( ${QT_USE_FILE} )

# --------------------------------------------------------------------
# intl
# --------------------------------------------------------------------
BUILDSYS_AUTO_LIBRARY_SOURCES(
    intl_SOURCES
    intl.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/intl.qrc
    intl.rc.in
)

ADD_LIBRARY( intl ${intl_SOURCES} )
ADD_DEPENDENCIES( intl lrelease )
TARGET_LINK_LIBRARIES( intl ${QT_LIBRARIES} )
SET_TARGET_PROPERTIES( intl PROPERTIES OUTPUT_NAME ${BUILDSYS_INTL_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_INTL_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS intl ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
