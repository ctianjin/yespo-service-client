# - Try to find WEBRTC
# Once done this will define
#
#  WEBRTC_FOUND - system has PJSIP
#  WEBRTC_INCLUDE_DIR - the WebRTC include directory
#  WEBRTC_INCLUDE_DIR_LIBRARIES - Link these to use WebRTC
#
# No version checking is done - use WEBRTC_API_VERSION_CURRENT to
# conditionally compile version-dependent code

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( WEBRTC_INCLUDE_DIR MediaCore.h )

FIND_LIBRARY( WEBRTC_LIBRARIES NAMES WEBRTC )

IF( WEBRTC_MEDIA_LIBRARIES )
    SET( WEBRTC_LIBRARIES ${WEBRTC_MEDIA_LIBRARIES} )
ENDIF( WEBRTC_MEDIA_LIBRARIES )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( WebRTC REQUIRED_VARS WEBRTC_LIBRARIES WEBRTC_INCLUDE_DIR )

# show the WEBRTC_INCLUDE_DIR and WEBRTC_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED( WEBRTC_INCLUDE_DIR WEBRTC_LIBRARIES )

