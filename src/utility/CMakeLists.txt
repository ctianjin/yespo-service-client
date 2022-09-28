# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_DONT_USE_QTGUI TRUE )

INCLUDE( ${QT_USE_FILE} )

# --------------------------------------------------------------------
# utility
# --------------------------------------------------------------------
BUILDSYS_AUTO_LIBRARY_SOURCES(
    utility_SOURCES
    abstracttask.cpp
    datetimerange.cpp
    endpoint.cpp
    executioncontext.cpp
    executionmonitor.cpp
    executionresult.cpp
    fileutility.cpp
    range.cpp
    signalblocker.cpp
    stringseparator.cpp
    taskrunnable.cpp
    version.cpp
    utility.rc.in
)

ADD_LIBRARY( utility ${utility_SOURCES} )

TARGET_LINK_LIBRARIES( utility ${QT_LIBRARIES} )

SET_TARGET_PROPERTIES( utility PROPERTIES OUTPUT_NAME ${BUILDSYS_UTILITY_OUTPUT_NAME} DEFINE_SYMBOL BUILDSYS_UTILITY_SOURCE )

IF( BUILD_SHARED_LIBS )
    INSTALL( TARGETS utility ${INSTALL_TARGETS_DEFAULT_ARGS} )
ENDIF( BUILD_SHARED_LIBS )
