/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_WB_SENSOR_H_
#define ZEPHYR_INCLUDE_WB_SENSOR_H_

/**
 * @brief set the wislblock sensor fetching interval
 *
 * @param interval period of the sample fetching in seconds.
 *
 */
void wisblock_set_fetching_interval(uint32_t interval);

#endif /* ZEPHYR_INCLUDE_WB_SENSOR_H_ */
