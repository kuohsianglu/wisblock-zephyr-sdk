# Copyright (c) 2025 Kenneth Lu
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(COMMON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../common)
include(${COMMON_PATH}/common.cmake)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(qthinger LANGUAGES C VERSION 1.0.0)

# determine git tag and commit hash for automatic firmware versioning
find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --long --dirty --tags
        OUTPUT_VARIABLE FIRMWARE_VERSION_ID
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(FIRMWARE_VERSION_ID "unknown")
endif()

add_definitions(-DFIRMWARE_VERSION_ID="${FIRMWARE_VERSION_ID}")

target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/include src)
target_sources_ifdef(CONFIG_BOARD_RAK4631_NRF52840 app PRIVATE boards/board.c)
target_sources(app PRIVATE
      src/main.c
      src/data_objects.c
      src/ts_sensor.c
)
