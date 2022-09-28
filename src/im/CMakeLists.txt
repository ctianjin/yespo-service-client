# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_USE_QTNETWORK TRUE )

INCLUDE( ${QT_USE_FILE} )


# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------


# --------------------------------------------------------------------
# im
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
	../utility
	${BUILDSYS_LIBS_DIR}/qjson/include
)

BUILDSYS_AUTO_LIBRARY_SOURCES(
    im_SOURCES
    im.cpp
	imcommand.cpp
	imretransqueue.cpp
	imudppackage.cpp
	imworkerthread.cpp
    im.rc.in
)

ADD_LIBRARY( im ${im_SOURCES} )
TARGET_LINK_LIBRARIES( im qjson ${QT_LIBRARIES} )


SET_TARGET_PROPERTIES( im PROPERTIES OUTPUT_NAME ${BUILDSYS_IM_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_IM_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS im ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
