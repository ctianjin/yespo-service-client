# - Try to find PJSIP
# Once done this will define
#
#  PJSIP_FOUND - system has PJSIP
#  PJSIP_PJLIB_INCLUDE_DIR - the pjlib include directory
#  PJSIP_PJLIB_UTIL_INCLUDE_DIR - the pjlib-util include directory
#  PJSIP_PJMEDIA_INCLUDE_DIR - the pjmedia include directory
#  PJSIP_PJNATH_INCLUDE_DIR - the pjnath include directory
#  PJSIP_PJSIP_INCLUDE_DIR - the pjsit include directory
#  PJSIP_LIBRARIES - Link these to use PJSIP
#
# No version checking is done - use PJSIP_API_VERSION_CURRENT to
# conditionally compile version-dependent code

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( PJSIP_PJLIB_INCLUDE_DIR pjlib.h )
FIND_PATH( PJSIP_PJLIB_UTIL_INCLUDE_DIR pjlib-util.h )
FIND_PATH( PJSIP_PJMEDIA_INCLUDE_DIR pjmedia.h )
FIND_PATH( PJSIP_PJNATH_INCLUDE_DIR pjnath.h )
FIND_PATH( PJSIP_PJSIP_INCLUDE_DIR pjsip.h )

FIND_LIBRARY( PJPROJECT_LIBRARIES NAMES PJPROJECT )

IF( PJPROJECT_LIBRARIES )
    SET( PJSIP_LIBRARIES ${PJPROJECT_LIBRARIES} )
ENDIF( PJPROJECT_LIBRARIES )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Pjsip REQUIRED_VARS PJSIP_LIBRARIES PJSIP_PJLIB_INCLUDE_DIR PJSIP_PJLIB_UTIL_INCLUDE_DIR PJSIP_PJMEDIA_INCLUDE_DIR PJSIP_PJNATH_INCLUDE_DIR PJSIP_PJSIP_INCLUDE_DIR )

# show the PJSIP_PJSIP_INCLUDE_DIR and PJSIP_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED( PJSIP_PJLIB_INCLUDE_DIR PJSIP_PJLIB_UTIL_INCLUDE_DIR PJSIP_PJMEDIA_INCLUDE_DIR PJSIP_PJNATH_INCLUDE_DIR PJSIP_PJSIP_INCLUDE_DIR PJPROJECT_LIBRARIES)

