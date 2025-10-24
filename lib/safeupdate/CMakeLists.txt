# SPDX-License-Identifier: Apache-2.0

zephyr_library_sources(mcumgr_img.c)
zephyr_library_sources_ifdef(CONFIG_SAFEUPDATE_BT mcumgr_bt.c)
