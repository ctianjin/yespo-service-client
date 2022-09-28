# $Id: FindQtMobility.cmake 310 2012-10-26 05:16:26Z twu $
#
# Copyright (C) 2012 China Science & Intelligence Information Technology Co.,Ltd.
# All rights reserved.
#
# Contact: Development Department <devel@cnsiit.com>
# Author: Joseph Wu <twu@cnsiit.com>
#

# Try to find the QtMobility libraries
#
# This module will search for the QtMobility libraries.
#
# It supports both a minimum version and searching for individual
# components. For the minimum version, use
# find_package(QtMobility 1.2.0). For specific components, use
# find_package(QtMobility COMPONENTS ...). See below for a list of known
# components.
#
# Once done this will define
#  QTMOBILITY_FOUND - QtMobility and all specified components were found.
#  QTMOBILITY_INCLUDE_DIR - Include directory for global QtMobility files.
#  QTMOBILITY_INCLUDE_DIRS - All found QtMobility components' include dirs.
#  QTMOBILITY_LIBRARIES - All found QtMobility components' libraries.
#  QTMOBILITY_VERSION - The version of QtMobility that was found.
#
# For each found component the following will be defined:
#  QTMOBILITY_{COMPONENT}_INCLUDE_DIR - The include directory for the component.
#  QTMOBILITY_{COMPONENT}_LIBRARY - The location of the library for the component.
#
# Note that searching for components will be limited to the specified components
# if the components option is used.

SET(
    QTMOBILITY_COMPONENTS
    Bearer
    Connectivity
    Contacts
    Feedback
    Gallery
    Location
    Messaging
    MultimediaKit
    Organizer
    PublishSubscribe
    Sensors
    ServiceFramework
    SystemInfo
    Versit
)

IF( QtMobility_FIND_COMPONENTS )
    FOREACH( component ${QtMobility_FIND_COMPONENTS} )
        STRING( TOUPPER ${component} _COMPONENT )
        SET( QTMOBILITY_USE_${_COMPONENT} 1 )
    ENDFOREACH( component )
ENDIF( QtMobility_FIND_COMPONENTS )

FIND_PATH( QTMOBILITY_INCLUDE_DIR qmobilityglobal.h PATH_SUFFIXES QtMobility )
FIND_PATH( QTMOBILITY_LIBRARY_DIR QtMultimediaKitd1.dll )
FIND_PATH( QTMOBILITY_PLUGINS_DIR mediaservice/dsengine.dll )
IF( NOT QTMOBILITY_INCLUDE_DIR AND QT4_FOUND )
    FIND_PATH( QTMOBILITY_INCLUDE_DIR qmobilityglobal.h PATH_SUFFIXES QtMobility PATHS ${QT_INCLUDE_DIR} )
ENDIF( NOT QTMOBILITY_INCLUDE_DIR AND QT4_FOUND )

# Find the mobility version
IF( QTMOBILITY_INCLUDE_DIR )
    IF( WIN32 )
        FILE( READ "${QTMOBILITY_INCLUDE_DIR}/QtMobility/qmobilityglobal.h" _qtmobility_global_header LIMIT 5000 )
    ELSE( WIN32 )
        FILE( READ "${QTMOBILITY_INCLUDE_DIR}/qmobilityglobal.h" _qtmobility_global_header LIMIT 5000 )
    ENDIF( WIN32 )
    STRING( REGEX MATCH "#define QTM_VERSION_STR \"([0-9.]*)\"" _qtmobility_version_match "${_qtmobility_global_header}" )
    SET( QTMOBILITY_VERSION "${CMAKE_MATCH_1}" )
ENDIF( QTMOBILITY_INCLUDE_DIR )

SET( QTMOBILITY_VARIABLES "QTMOBILITY_INCLUDE_DIR" )
IF( WIN32 )
    SET( QTMOBILITY_INCLUDE_DIRS ${QTMOBILITY_INCLUDE_DIR} ${QTMOBILITY_INCLUDE_DIR}/QtMobility )
ELSE( WIN32 )
    SET( QTMOBILITY_INCLUDE_DIRS ${QTMOBILITY_INCLUDE_DIR} )
ENDIF( WIN32 )

# A list of files to find for specific components
SET(
    QTMOBILITY_FIND_FILES
    QNetworkConfiguration # Bearer
    QBluetoothSocket # Connectivity
    QContact # Contacts
    QFeedbackInterface # Feedback
    QAbstractGallery # Gallery
    QLandmark # Location
    QMessage # Messaging
    QMediaPlayer # MultimediaKit
    QOrganizerItem # Organizer
    QValueSpace # PublishSubscribe
    QSensor # Sensors
    QService # ServiceFramework
    QSystemInfo # SystemInfo
    QVersitDocument # Versit
)

LIST( LENGTH QTMOBILITY_COMPONENTS _component_count )
MATH( EXPR _component_count "${_component_count} - 1" )
FOREACH( index RANGE ${_component_count} )
    LIST( GET QTMOBILITY_COMPONENTS ${index} component )
    LIST( GET QTMOBILITY_FIND_FILES ${index} file )
    STRING( TOUPPER ${component} _COMPONENT )

    IF( NOT QtMobility_FIND_COMPONENTS OR QTMOBILITY_USE_${_COMPONENT} )
        FIND_PATH( QTMOBILITY_${_COMPONENT}_INCLUDE_DIR ${file} PATH_SUFFIXES Qt${component} qt4/Qt${component} )
        FIND_LIBRARY( QTMOBILITY_${_COMPONENT}_LIBRARY NAMES Qt${component} )

        LIST( APPEND QTMOBILITY_VARIABLES "QTMOBILITY_${_COMPONENT}_INCLUDE_DIR" "QTMOBILITY_${_COMPONENT}_LIBRARY" )
        LIST( APPEND QTMOBILITY_INCLUDE_DIRS ${QTMOBILITY_${_COMPONENT}_INCLUDE_DIR} )
        LIST( APPEND QTMOBILITY_LIBRARIES ${QTMOBILITY_${_COMPONENT}_LIBRARY} )
    ENDIF( NOT QtMobility_FIND_COMPONENTS OR QTMOBILITY_USE_${_COMPONENT} )
ENDFOREACH( index )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( QtMobility REQUIRED_VARS ${QTMOBILITY_VARIABLES} VERSION_VAR QTMOBILITY_VERSION )

MARK_AS_ADVANCED( ${QTMOBILITY_VARIABLES} )
