/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_ZBUS_EXT_LIB_H_
#define ZEPHYR_INCLUDE_ZBUS_EXT_LIB_H_

#include <stdint.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/drivers/sensor.h>

#define SENSOR_ZMSG_BUF_SIZE	  32
#define SENSOR_ZMSG_RAW_DATA_TYPE 254
#define MAF_DATA_LEN		  32
#define BLE_ADDR_TYPE_PUBLIC	  "public"
#define BLE_ADDR_TYPE_RANDOM	  "random"

struct sensor_zmsg {
	uint8_t conn_index;
	uint8_t type;
	struct sensor_value data;
	uint8_t buf[SENSOR_ZMSG_BUF_SIZE];
	uint8_t buf_size;
};

#endif /* ZEPHYR_INCLUDE_ZBUS_EXT_LIB_H_ */
