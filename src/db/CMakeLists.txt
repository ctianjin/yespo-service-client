# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_DONT_USE_QTGUI TRUE )
SET( QT_USE_QTSQL TRUE )

INCLUDE( ${QT_USE_FILE} )


# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
IF( BUILDSYS_DB_LOG_SQL )
    ADD_DEFINITIONS( -DBUILDSYS_DB_LOG_SQL )
ENDIF( BUILDSYS_DB_LOG_SQL )

# --------------------------------------------------------------------
# db
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
    ../utility
)

BUILDSYS_AUTO_LIBRARY_SOURCES(
    db_SOURCES
    abstractdatabasedaos.cpp
    abstractdatabasemodel.cpp
    abstractentitydao.cpp
    abstractentitymodel.cpp
    abstractentitytreemodel.cpp
    database.cpp
    databaseregistrar.cpp
    entitydata.cpp
    genericdao.cpp
    genericidentitydao.cpp
    identityrecord.cpp
    memorydatabase.cpp
    oracledatabase.cpp
    record.cpp
    recordaccessor.cpp
    sqlitedatabase.cpp
    transaction.cpp
    db.rc.in
)

ADD_LIBRARY( db ${db_SOURCES} )
TARGET_LINK_LIBRARIES( db utility ${QT_LIBRARIES} )
SET_TARGET_PROPERTIES( db PROPERTIES OUTPUT_NAME ${BUILDSYS_DB_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_DB_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS db ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
