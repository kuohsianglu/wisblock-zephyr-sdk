# SPDX-License-Identifier: Apache-2.0

zephyr_library_sources_ifdef(CONFIG_WISBLOCK_SENSOR wb_sensor.c wb_shell.c)
zephyr_library_sources_ifdef(CONFIG_WISBLOCK_IO wb_io.c)
