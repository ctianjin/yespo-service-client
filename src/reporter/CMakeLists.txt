# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
IF( WIN32 )
    SET( QT_USE_QTMAIN TRUE )
ENDIF( WIN32 )

INCLUDE( ${QT_USE_FILE} )

# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------

# --------------------------------------------------------------------
# reporter
# --------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    .
	${SMTP_INCLUDE_DIR}
)

BUILDSYS_AUTO_SOURCES(
    reporter_SOURCES
	main.cpp
	reporter.rc.in
)

ADD_EXECUTABLE( reporter WIN32 ${reporter_SOURCES} )

TARGET_LINK_LIBRARIES( reporter ${QT_LIBRARIES} ${SMTP_LIBRARIES} )

SET_TARGET_PROPERTIES( reporter PROPERTIES OUTPUT_NAME ${BUILDSYS_REPORTER_OUTPUT_NAME} )

INSTALL( TARGETS reporter ${INSTALL_TARGETS_DEFAULT_ARGS} )
