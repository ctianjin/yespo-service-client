# --------------------------------------------------------------------
# Qt      
# --------------------------------------------------------------------
SET( QT_USE_QTNETWORK TRUE )
#SET( QT_USE_QTWEBKIT TRUE ) ### For media service that download from web storage
SET( QT_USE_QTMULTIMEDIA TRUE )
# IF( WIN32 )
#     SET( QT_USE_QAXCONTAINER TRUE )
# ENDIF( WIN32 )

INCLUDE( ${QT_USE_FILE} )


# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
IF( BUILDSYS_SERVICE_USE_STRICT_RESUME )
    ADD_DEFINITIONS( -DBUILDSYS_SERVICE_USE_STRICT_RESUME )
ENDIF( BUILDSYS_SERVICE_USE_STRICT_RESUME )

# --------------------------------------------------------------------
# Databases
# --------------------------------------------------------------------
BUILDSYS_ADD_DATABASE_MODULE( service user media )

# --------------------------------------------------------------------
# service
# --------------------------------------------------------------------

INCLUDE_DIRECTORIES(
    ../pref
    ../db
    ../utility
    ${BUILDSYS_LIBS_DIR}/qjson/include
	${PJSIP_PJLIB_INCLUDE_DIR}
	${PJSIP_PJLIB_UTIL_INCLUDE_DIR}
	${PJSIP_PJMEDIA_INCLUDE_DIR}
	${PJSIP_PJNATH_INCLUDE_DIR}
	${PJSIP_PJSIP_INCLUDE_DIR}
	${WEBRTC_INCLUDE_DIR}
	${QTMOBILITY_INCLUDE_DIRS}
)

BUILDSYS_AUTO_LIBRARY_SOURCES(
    service_SOURCES
    abstractdataservice.cpp
    abstractservice.cpp
    service.cpp
    servicemanager.cpp
    userservice.cpp
#	sipcommand.cpp
	sipservice.cpp
	audioservice.cpp
	command.cpp
	downloadthread.cpp
	downloadqueue.cpp
    commandservice.cpp
	mediaservice.cpp
	cameraservice.cpp
	photoservice.cpp
	downloadthread.cpp
    ${servicedb_module_SOURCES}
    service.rc.in
)

ADD_LIBRARY( service ${service_SOURCES} )

TARGET_LINK_LIBRARIES( service pref db utility qjson ${QT_LIBRARIES} wsock32.lib ws2_32.lib ${DXSDK_DSOUND_LIBRARY} ${PJSIP_LIBRARIES} ${WEBRTC_LIBRARIES} ${QTMOBILITY_LIBRARIES})

SET_TARGET_PROPERTIES( service PROPERTIES OUTPUT_NAME ${BUILDSYS_SERVICE_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_SERVICE_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS service ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
