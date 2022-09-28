# - Try to find VoIP SDK
# Once done this will define
#
#  VOIPSDK_FOUND - system has PJSIP
#  VOIPSDK_INCLUDE_DIR - the WebRTC include directory
#  VOIPSDK_INCLUDE_DIR_LIBRARIES - Link these to use WebRTC
#
# No version checking is done - use VOIPSDK_API_VERSION_CURRENT to
# conditionally compile version-dependent code

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( VOIPSDK_INCLUDE_DIR sipwebrtc.h )
FIND_PATH( PJSIP_PJLIB_INCLUDE_DIR pjlib.h )
FIND_PATH( PJSIP_PJLIB_UTIL_INCLUDE_DIR pjlib-util.h )
FIND_PATH( PJSIP_PJMEDIA_INCLUDE_DIR pjmedia.h )
FIND_PATH( PJSIP_PJNATH_INCLUDE_DIR pjnath.h )
FIND_PATH( PJSIP_PJSIP_INCLUDE_DIR pjsip.h )

FIND_LIBRARY( VOIPSDK_LIBRARIES NAMES VOIPSDK )

IF( VOIPSDK_MEDIA_LIBRARIES )
    SET( VOIPSDK_LIBRARIES ${VOIPSDK_MEDIA_LIBRARIES} )
ENDIF( VOIPSDK_MEDIA_LIBRARIES )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( VoIPSDK REQUIRED_VARS VOIPSDK_LIBRARIES VOIPSDK_INCLUDE_DIR PJSIP_PJLIB_INCLUDE_DIR PJSIP_PJLIB_UTIL_INCLUDE_DIR PJSIP_PJMEDIA_INCLUDE_DIR PJSIP_PJNATH_INCLUDE_DIR PJSIP_PJSIP_INCLUDE_DIR)

# show the VOIPSDK_INCLUDE_DIR and VOIPSDK_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED( VOIPSDK_INCLUDE_DIR VOIPSDK_LIBRARIES )

