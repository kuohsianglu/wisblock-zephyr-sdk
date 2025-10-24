/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include "data_objects.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/sys/crc.h>
#include <zephyr/sys/reboot.h>
#include <thingset/sdk.h>
#include <thingset/storage.h>
#include <stdio.h>
#include <math.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(data_objs);

static char manufacturer[] = "RakThingSet";
static char firmware_version[] = FIRMWARE_VERSION_ID;
static float room_temp;
static float humidity;
static float target_temp = 22.0F;
static float OFFSET = 25.0f;
static float SCALING_FACTOR = 2.0f;
static bool heater_on;

/**
 * ThingSet data objects (see https://github.com/ThingSet)
 */

/* DEVICE INFORMATION */

THINGSET_ADD_GROUP(TS_ID_ROOT, APP_ID_DEVICE, "Device", THINGSET_NO_CALLBACK);

THINGSET_ADD_ITEM_STRING(APP_ID_DEVICE, APP_ID_DEVICE_MANUFACTURER, "cManufacturer", manufacturer,
			 sizeof(manufacturer), THINGSET_ANY_R, 0);

THINGSET_ADD_ITEM_STRING(APP_ID_DEVICE, APP_ID_DEVICE_FW_VER, "cFirmwareVersion", firmware_version,
			 sizeof(firmware_version), THINGSET_ANY_R, 0);

THINGSET_ADD_FN_VOID(APP_ID_DEVICE, APP_ID_DEVICE_RESET, "xReset", &reset_device, THINGSET_ANY_RW);

/* SENSOR DATA */
THINGSET_ADD_GROUP(TS_ID_ROOT, APP_ID_SENSOR, "Sensor", THINGSET_NO_CALLBACK);

THINGSET_ADD_ITEM_FLOAT(APP_ID_SENSOR, APP_ID_SENSOR_TEMP, "rRoomTemp_degC", &room_temp, 1,
			THINGSET_ANY_R, TS_SUBSET_SUMMARY);

THINGSET_ADD_ITEM_FLOAT(APP_ID_SENSOR, APP_ID_SENSOR_HUMIDITY, "rHumidity_pct", &humidity, 1,
			THINGSET_ANY_R, TS_SUBSET_SUMMARY);

/* CONTROL */
THINGSET_ADD_GROUP(TS_ID_ROOT, APP_ID_CONTROL, "Control", data_objects_update_conf);

THINGSET_ADD_ITEM_BOOL(APP_ID_CONTROL, APP_ID_CONTROL_HEATER_ON, "rHeaterOn", &heater_on,
		       THINGSET_ANY_R, TS_SUBSET_LIVE);

THINGSET_ADD_ITEM_FLOAT(APP_ID_CONTROL, APP_ID_CONTROL_TARGET_TEMP, "sTargetTemp_degC",
			&target_temp, 1, THINGSET_ANY_RW, TS_SUBSET_NVM);

static uint8_t data_objects_encode_sensor_val(float sensor_val)
{
	float calculated_value = (sensor_val + OFFSET) * SCALING_FACTOR;

	return roundf(calculated_value);
}

void reset_device(void)
{
	sys_reboot(SYS_REBOOT_COLD);
}

void data_objects_update_conf(enum thingset_callback_reason reason)
{
	if (reason == THINGSET_CALLBACK_POST_WRITE) {
		LOG_INF("%s", __func__);
	}
}

void data_objects_sensor_data(struct sensor_zmsg *s_zmsg)
{
	switch (s_zmsg->type) {
	case SENSOR_CHAN_AMBIENT_TEMP:
		room_temp = sensor_value_to_double(&s_zmsg->data);
		break;
	case SENSOR_CHAN_HUMIDITY:
		humidity = sensor_value_to_double(&s_zmsg->data);
		break;
	default:
		break;
	}
}

uint8_t data_objects_get_sensor_value(uint8_t sensor_type)
{
	uint8_t val = 0;

	switch (sensor_type) {
	case SENSOR_CHAN_AMBIENT_TEMP:
		val = data_objects_encode_sensor_val(room_temp);
		break;
	case SENSOR_CHAN_HUMIDITY:
		val = data_objects_encode_sensor_val(humidity);
		break;
	default:
		break;
	}

	return val;
}
