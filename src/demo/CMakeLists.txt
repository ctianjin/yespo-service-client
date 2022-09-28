# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
IF( WIN32 )
    SET( QT_USE_QTMAIN TRUE )
ENDIF( WIN32 )

INCLUDE( ${QT_USE_FILE} )

# --------------------------------------------------------------------
# demo
# --------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    .
    ../proto
)

BUILDSYS_AUTO_SOURCES(
    demo_SOURCES
    main.cpp
	mainwidget.cpp
	mainwidget.ui
	demo.qrc
	demo.rc.in
)

ADD_EXECUTABLE( demo WIN32 ${demo_SOURCES} )

TARGET_LINK_LIBRARIES( demo proto ${QT_LIBRARIES} )

SET_TARGET_PROPERTIES( demo PROPERTIES OUTPUT_NAME "Demo" )

INSTALL( TARGETS demo ${INSTALL_TARGETS_DEFAULT_ARGS} )
