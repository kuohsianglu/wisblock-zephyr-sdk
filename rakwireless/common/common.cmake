# Copyright (c) 2025 Kenneth Lu
#
# SPDX-License-Identifier: Apache-2.0

if(BOARD AND EXISTS ${CMAKE_SOURCE_DIR}/boards/${BOARD}.overlay)
  set(DTC_OVERLAY_FILE ${CMAKE_SOURCE_DIR}/boards/${BOARD}.overlay)
endif()

set(WISBLOCK_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/wisblock/wb.cmake)

if(${CONFIG_WISBLOCK})
  include(${WISBLOCK_CMAKE})
endif()
