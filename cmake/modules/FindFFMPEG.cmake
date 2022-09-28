# - Try to find FFMPEG
# Once done this will define
#
#  FFMPEG_FOUND - system has FFMPEG
#  FFMPEG_INCLUDE_DIR - the FFMPEG include directory
#  FFMPEG_LIBRARIES - Link these to use FFMPEG
#
# No version checking is done - use FFMPEG_API_VERSION_CURRENT to
# conditionally compile version-dependent code

# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( FFMPEG_INCLUDE_DIR libavcodec )

FIND_LIBRARY( FFMPEGPROJECT_LIBRARIES NAMES FFMPEGPROJECT )

IF( FFMPEGPROJECT_LIBRARIES )
    SET( FFMPEG_LIBRARIES ${FFMPEGPROJECT_LIBRARIES} )
ENDIF( FFMPEGPROJECT_LIBRARIES )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( FFMPEG REQUIRED_VARS FFMPEG_LIBRARIES FFMPEG_INCLUDE_DIR )

# show the FFMPEG_FFMPEG_INCLUDE_DIR and FFMPEG_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED( FFMPEG_INCLUDE_DIR FFMPEGPROJECT_LIBRARIES)

