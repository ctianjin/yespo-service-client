PROJECT( VoiceEasy )

CMAKE_MINIMUM_REQUIRED( VERSION 2.8.6 )

SET( BUILDSYS_VENDOR_NAME "RAR Limited" )
SET( BUILDSYS_VENDOR_DOMAIN "yespo.com" )
SET( BUILDSYS_PRODUCT_NAME "YesPo" )
SET( BUILDSYS_PRODUCT_DOMAIN "YesPo" )

SET( BUILDSYS_VERSION_MAJOR "1" )
SET( BUILDSYS_VERSION_MINOR "1" )
SET( BUILDSYS_VERSION_PATCH "95" )
SET( BUILDSYS_VERSION "${BUILDSYS_VERSION_MAJOR}.${BUILDSYS_VERSION_MINOR}.${BUILDSYS_VERSION_PATCH}" )

SET( BUILDSYS_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )
SET( BUILDSYS_CMAKE_DIR "${BUILDSYS_ROOT_DIR}/cmake" )
SET( BUILDSYS_CMAKE_MODULES_DIR "${BUILDSYS_CMAKE_DIR}/modules" )
SET( BUILDSYS_SRC_DIR "${BUILDSYS_ROOT_DIR}/src" )
SET( BUILDSYS_TEST_DIR "${BUILDSYS_ROOT_DIR}/test" )
SET( BUILDSYS_TOOLS_DIR "${BUILDSYS_ROOT_DIR}/tools" )
SET( BUILDSYS_LIBS_DIR "${BUILDSYS_ROOT_DIR}/libs" )

SET( CMAKE_MODULE_PATH ${BUILDSYS_CMAKE_MODULES_DIR} ${CMAKE_MODULE_PATH} )

INCLUDE( ${BUILDSYS_CMAKE_DIR}/revision.cmake )

# --------------------------------------------------------------------
# Options
# --------------------------------------------------------------------
OPTION( BUILD_SHARED_LIBS "Build Shared Libraries" ON )
OPTION( BUILDSYS_ENABLE_SINGLE_APPLICATION "Only allow a single application instance to run" OFF )
OPTION( BUILDSYS_ENABLE_CREATE_TRANSLATIONS "Create translations (lupdate target)" OFF )
OPTION( BUILDSYS_NO_OBSOLETE_TRANSLATIONS "Add -no-obsolete option to lupdate" OFF )
OPTION( BUILDSYS_PHONE_ENABLE_TRANSLUCENT "Enables translucent background of main window" OFF )
OPTION( BUILDSYS_PHONE_ENABLE_TRANSLUCENT_WORKAROUND "Enable workaround on translucent issue" OFF )
OPTION( BUILDSYS_PHONE_LOGGING "Log the message to a file" OFF)
OPTION( BUILDSYS_UI_USE_CLEANLOOKS "Use Cleanlooks style" OFF )
OPTION( BUILDSYS_UI_USE_QTCURVE "Use QtCurve style" OFF )
OPTION( BUILDSYS_PHONE_DYNAMIC_SERVER "Settings for dynamic server" OFF )
OPTION( BUILDSYS_PACKAGE_PHONE "Make package voice easy phone" ON )
OPTION( BUILDSYS_BUILD_TEST "Build test project" OFF )
OPTION( BUILDSYS_DEMO "Demo" OFF)
OPTION(	BUILDSYS_LOCAL "Local" OFF)
OPTION( BUILDSYS_OFFICAL "Offical" ON)

IF( WIN32 )
	SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:NOREF")
    OPTION( BUILDSYS_ENABLE_ACTIVEQT_WORKAROUND "Enable workaround for ActiveQt" ON )
ENDIF( WIN32 )

SET( BUILDSYS_UPDATE_URL "" CACHE STRING "Update URL" )

# --------------------------------------------------------------------
# Qt
# --------------------------------------------------------------------
SET( QT_MIN_VERSION 4.7.1)
FIND_PACKAGE( Qt4 REQUIRED )
SET( CMAKE_AUTOMOC ON )

FIND_PACKAGE( Pjsip )
FIND_PACKAGE( DXSDK )
FIND_PACKAGE( Webrtc )
FIND_PACKAGE( QtMobility COMPONENTS MultimediaKit SystemInfo )
FIND_PACKAGE( CrashHandler )
FIND_PACKAGE( SMTP )

# --------------------------------------------------------------------
# Boost
# --------------------------------------------------------------------
SET( Boost_USE_MULTITHREADED ON )
OPTION( Boost_USE_STATIC_LIBS "Use static Boost libraries" ON )
OPTION( Boost_USE_STATIC_RUNTIME "Use Boost libraries compiled with static runtime" OFF )
FIND_PACKAGE( Boost 1.44 COMPONENTS date_time filesystem program_options system REQUIRED )

# --------------------------------------------------------------------
# Modules
# --------------------------------------------------------------------
INCLUDE( ${BUILDSYS_CMAKE_DIR}/defaults.cmake )
INCLUDE( ${BUILDSYS_CMAKE_DIR}/macros.cmake )

# --------------------------------------------------------------------
# Third-party libraries
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( libs )

# --------------------------------------------------------------------
# Tools
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( tools )

# --------------------------------------------------------------------
# Main sources
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( src )

# --------------------------------------------------------------------
# Test
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( test )

# --------------------------------------------------------------------
# Package
# --------------------------------------------------------------------
ADD_SUBDIRECTORY( package )

# --------------------------------------------------------------------
# Additional resources
# --------------------------------------------------------------------
#INSTALL(
#    FILES ChangeLog COPYING README
#    DESTINATION "${DOC_INSTALL_DIR}"
#)
