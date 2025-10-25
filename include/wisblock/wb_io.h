/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_WB_IO_H_
#define ZEPHYR_INCLUDE_WB_IO_H_

#define WB_IO_INIT_PRIORITY_DEFAULT 60

/**
 * @brief WisBlock init function
 *
 * @return 0 if successful, negative errno code if failure
 */
int wisblock_io_relay_onoff(void);

#endif /* ZEPHYR_INCLUDE_WB_IO_H_ */
