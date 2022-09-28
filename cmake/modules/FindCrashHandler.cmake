FIND_PATH( CRASHHANDLER_INCLUDE_DIR CrashHandler.h )

FIND_LIBRARY( CRASHHANDLER_LIBRARIES NAMES CRASHHANDLER )

IF( CRASHHANDLER_LIBRARIES )
    SET( CRASHHANDLER_LIBRARIES ${CRASHHANDLER_LIBRARIES} )
ENDIF( CRASHHANDLER_LIBRARIES )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( CRASHHANDLER REQUIRED_VARS CRASHHANDLER_LIBRARIES CRASHHANDLER_INCLUDE_DIR )

# show the CRASHHANDLER_INCLUDE_DIR and CRASHHANDLER_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED( CRASHHANDLER_INCLUDE_DIR CRASHHANDLER_LIBRARIES )