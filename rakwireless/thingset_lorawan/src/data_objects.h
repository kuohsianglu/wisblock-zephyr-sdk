/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DATA_OBJECTS_H_
#define DATA_OBJECTS_H_

/**
 * @file
 * @brief Handling of ThingSet data nodes
 */

#include <thingset.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zbus_ext/zbus_ext.h>

/*
 * Categories / first layer node IDs
 */

/* Device information (e.g. manufacturer, etc.) */
#define APP_ID_DEVICE		   0x04
#define APP_ID_DEVICE_MANUFACTURER 0x40
#define APP_ID_DEVICE_TYPE	   0x41
#define APP_ID_DEVICE_HW_VER	   0x42
#define APP_ID_DEVICE_FW_VER	   0x43
#define APP_ID_DEVICE_SHUTDOWN	   0x4A
#define APP_ID_DEVICE_RESET	   0x4B

/* Sensor data */
#define APP_ID_SENSOR	       0x05
#define APP_ID_SENSOR_TEMP     0x50
#define APP_ID_SENSOR_HUMIDITY 0x51

/* Control data */
#define APP_ID_CONTROL		   0x06
#define APP_ID_CONTROL_HEATER_ON   0x60
#define APP_ID_CONTROL_TARGET_TEMP 0x61

/* Openhaystack data */
#define APP_ID_OHS 0x07

/**
 * Callback to reset device (obviously...)
 */
void reset_device(void);

/**
 * Callback function to be called when conf values were changed
 */
void data_objects_update_conf(enum thingset_callback_reason reason);

/**
 * function to update sensor data from zbus
 */
void data_objects_sensor_data(struct sensor_zmsg *s_zmsg);

/**
 * function to get sensor value by type
 */
uint8_t data_objects_get_sensor_value(uint8_t sensor_type);

#endif /* DATA_OBJECTS_H_ */
