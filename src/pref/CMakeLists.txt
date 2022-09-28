# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
# SET( QT_DONT_USE_QTGUI TRUE )

INCLUDE( ${QT_USE_FILE} )


# --------------------------------------------------------------------
# Databases
# --------------------------------------------------------------------
BUILDSYS_ADD_DATABASE_MODULE( pref pref )

# --------------------------------------------------------------------
# pref
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ../db
    ../utility
)

BUILDSYS_AUTO_LIBRARY_SOURCES(
    pref_SOURCES
    abstractpreference.cpp
    defaultpreference.cpp
    preference.cpp
    preferencemanager.cpp
    preferencesupport.cpp
    preferencetransaction.cpp
    ${prefdb_module_SOURCES}
    pref.rc.in
)

ADD_LIBRARY( pref ${pref_SOURCES} )
TARGET_LINK_LIBRARIES( pref db utility ${QT_LIBRARIES} )
SET_TARGET_PROPERTIES( pref PROPERTIES OUTPUT_NAME ${BUILDSYS_PREF_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_PREF_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS pref ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
