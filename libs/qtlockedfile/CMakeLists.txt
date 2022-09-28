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
# qtlockedfile
# --------------------------------------------------------------------
SET( qtlockedfile_SOURCES qtlockedfile.cpp )

IF( UNIX )
    SET( qtlockedfile_SOURCES ${qtlockedfile_SOURCES} qtlockedfile_unix.cpp )
ENDIF( UNIX )

IF( WIN32 )
    ADD_DEFINITIONS( -DUNICODE )
    SET( qtlockedfile_SOURCES ${qtlockedfile_SOURCES} qtlockedfile_win.cpp )
ENDIF( WIN32 )

ADD_LIBRARY( qtlockedfile STATIC ${qtlockedfile_SOURCES} )
IF( USE_QT5 )
	QT5_USE_MODULES( qtlockedfile Core )
ELSE()
	TARGET_LINK_LIBRARIES( qtlockedfile ${QT_LIBRARIES} )
ENDIF()
SET_TARGET_PROPERTIES( qtlockedfile PROPERTIES AUTOMOC FALSE )
