# Copyright (c) 2025 Kenneth Lu
#
# SPDX-License-Identifier: Apache-2.0

set(WISBLOCK_BOARDS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/wisblock/boards)

if(BOARD AND EXISTS ${DTC_OVERLAY_FILE})
  if(${CONFIG_WISBLOCK_SENSOR_RAK1904})
    list(APPEND DTC_OVERLAY_FILE ${WISBLOCK_BOARDS_DIR}/${BOARD}_wb_rak1904.overlay)
  endif()
  if(${CONFIG_WISBLOCK_SENSOR_RAK1901})
    list(APPEND DTC_OVERLAY_FILE ${WISBLOCK_BOARDS_DIR}/${BOARD}_wb_rak1901.overlay)
  endif()
  if(${CONFIG_WISBLOCK_IO_RAK13007})
    list(APPEND DTC_OVERLAY_FILE ${WISBLOCK_BOARDS_DIR}/${BOARD}_wb_rak13007.overlay)
  endif()
endif()
