/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zbus_ext/zbus_ext.h>
#ifdef CONFIG_CAYENNELPP
#include <cayennelpp/cayennelpp.h>
#endif

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wb_sensor);

#define PRIORITY 7

ZBUS_CHAN_DECLARE(sensor_data_chan);

static struct sensor_zmsg s_zmsg;
static uint32_t data_fetching_period = CONFIG_WISBLOCK_SENSOR_FETCH_INTERVAL;

void wisblock_set_fetching_interval(uint32_t interval)
{
	data_fetching_period = interval;
}

static void wisblock_sensor_fetch(const struct device *sensor)
{
	if (IS_ENABLED(CONFIG_WISBLOCK_SENSOR_RAK1901)) {
		struct sensor_value temp, humidity;

		sensor_sample_fetch(sensor);

		sensor_channel_get(sensor, SENSOR_CHAN_HUMIDITY, &humidity);
		s_zmsg.data = humidity;
		s_zmsg.type = SENSOR_CHAN_HUMIDITY;
		zbus_chan_pub(&sensor_data_chan, &s_zmsg, K_SECONDS(1));

		k_sleep(K_MSEC(1000));

		sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		s_zmsg.data = temp;
		s_zmsg.type = SENSOR_CHAN_AMBIENT_TEMP;
		zbus_chan_pub(&sensor_data_chan, &s_zmsg, K_SECONDS(1));

		LOG_INF("T: %d.%06d; H: %d.%06d;", temp.val1, temp.val2, humidity.val1,
			humidity.val2);
	}

	if (IS_ENABLED(CONFIG_WISBLOCK_SENSOR_RAK1904)) {
		struct sensor_value accel[3];
		struct sensor_value temperature;
		int rc = sensor_sample_fetch(sensor);

		if (rc == 0) {
			rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
			if (rc < 0) {
				LOG_ERR("ERROR: Update failed: %d\n", rc);
			} else {
				size_t s_len = sizeof(struct sensor_value);

				s_zmsg.data = accel[0];
				s_zmsg.type = SENSOR_CHAN_ACCEL_XYZ;
				memcpy(s_zmsg.buf, &accel[1], s_len);
				memcpy(s_zmsg.buf + s_len, &accel[2], s_len);
				zbus_chan_pub(&sensor_data_chan, &s_zmsg, K_SECONDS(1));

				LOG_INF("3-Axis X: %d.%06d; Y: %d.%06d; Z: %d.%06d;", accel[0].val1,
					accel[0].val2, accel[1].val1, accel[1].val2, accel[2].val1,
					accel[2].val2);
			}
		}

		if (IS_ENABLED(CONFIG_LIS2DH_MEASURE_TEMPERATURE)) {
			if (rc == 0) {
				rc = sensor_channel_get(sensor, SENSOR_CHAN_DIE_TEMP, &temperature);
				if (rc < 0) {
					LOG_ERR("\nERROR: Unable to read temperature:%d\n", rc);
				} else {
					LOG_INF("T: %d.%06d", temperature.val1, temperature.val2);
				}
			}
		}
	}
}

static void wisblock_sensor_start(void)
{
	const struct device *dev = NULL;
#ifdef CONFIG_WISBLOCK_SENSOR_RAK1901
	dev = DEVICE_DT_GET_ONE(sensirion_shtcx);
#endif
#ifdef CONFIG_WISBLOCK_SENSOR_RAK1904
	dev = DEVICE_DT_GET_ANY(st_lis2dh);
#endif

	if (!device_is_ready(dev)) {
		LOG_ERR("sensor: device not ready");
		return;
	}

	LOG_INF("Sensor Device %p name %s", dev, dev->name);

	while (1) {
		k_sleep(K_SECONDS(data_fetching_period));
		wisblock_sensor_fetch(dev);
	}
}

K_THREAD_DEFINE(wb_sensor, 1024, wisblock_sensor_start, NULL, NULL, NULL, PRIORITY, 0, 0);
