# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_USE_QTNETWORK TRUE )
SET( QT_DONT_USE_QTGUI TRUE )

FIND_PACKAGE( protobuf )

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
	${PROTOBUF_INCLUDE_DIR}
)

# --------------------------------------------------------------------
# proto
# --------------------------------------------------------------------
INCLUDE_DIRECTORIES(
	${BUILDSYS_LIBS_DIR}/qjson/include
)

BUILDSYS_AUTO_LIBRARY_SOURCES(
    proto_SOURCES
	yespoproto.cpp
	yespo_protocol.pb.cc
	package.cpp
	request.cpp
    proto.rc.in
)

ADD_LIBRARY( proto ${proto_SOURCES} )

TARGET_LINK_LIBRARIES( proto qjson ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} wsock32.lib ws2_32.lib )

SET_TARGET_PROPERTIES( proto PROPERTIES OUTPUT_NAME ${BUILDSYS_PROTO_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_PROTO_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS proto ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
