# - Try to find protobuf
# Once done this will define
#
#  PROTOBUF_FOUND - system has protobuf
#  PROTOBUF_INCLUDE_DIR - the protobuf include directory
#  PROTOBUF_LIBRARIES - Link these to use protobuf
#
# Copyright (c) 2006, Laurent Montel, <montel@kde.org>
# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

FIND_PATH( PROTOBUF_INCLUDE_DIR google/protobuf/stubs/common.h )
	
FIND_LIBRARY( PROTOBUF_LIBRARY NAMES libprotobuf.lib )
FIND_LIBRARY( PROTOBUF_LITE_LIBRARY NAMES libprotobuf-lite.lib )
FIND_LIBRARY( PROTOBUFC_LIBRARY NAMES libprotoc.lib )

LIST( APPEND PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARY} )
LIST( APPEND PROTOBUF_LIBRARIES ${PROTOBUF_LITE_LIBRARY} )
LIST( APPEND PROTOBUF_LIBRARIES ${PROTOBUFC_LIBRARY} )

INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( protobuf REQUIRED_VARS PROTOBUF_LIBRARIES PROTOBUF_INCLUDE_DIR )

MARK_AS_ADVANCED( PROTOBUF_INCLUDE_DIR PROTOBUF_LIBRARIES)