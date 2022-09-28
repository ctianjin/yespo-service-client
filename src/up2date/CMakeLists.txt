# $Id: CMakeLists.txt 387 2012-12-11 10:13:15Z twu $
#
# Copyright (C) 2012 China Science & Intelligence Information Technology Co.,Ltd.
# All rights reserved.
#
# Contact: Development Department <devel@cnsiit.com>
# Author: Joseph Wu <twu@cnsiit.com>
#

# --------------------------------------------------------------------
# Configurations
# --------------------------------------------------------------------
CONFIGURE_FILE( XLiveUpdate.ini.in XLiveUpdate.ini )

# --------------------------------------------------------------------
# up2date
# --------------------------------------------------------------------
SET( up2date_RUNTIME ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/Up2date.exe )
SET( up2date_INI ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/XLiveUpdate.ini )

ADD_CUSTOM_COMMAND(
    OUTPUT ${up2date_RUNTIME}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Up2date.exe ${up2date_RUNTIME}
    VERBATIM
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${up2date_INI}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/XLiveUpdate.ini ${up2date_INI}
    VERBATIM
)

ADD_CUSTOM_TARGET( up2date DEPENDS ${up2date_RUNTIME} ${up2date_INI} )

INSTALL(
    FILES Up2date.exe ${CMAKE_CURRENT_BINARY_DIR}/XLiveUpdate.ini
    DESTINATION ${BIN_INSTALL_DIR}
    COMPONENT RUNTIME
)
