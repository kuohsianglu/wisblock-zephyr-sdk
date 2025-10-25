/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <thingset.h>
#include <thingset/sdk.h>
#include <zephyr/kernel.h>
#include "data_objects.h"

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ts_sensor);

#define PRIORITY 7

ZBUS_CHAN_DEFINE(sensor_data_chan,   /* Name */
		 struct sensor_zmsg, /* Message type */

		 NULL,			       /* Validator */
		 NULL,			       /* User data */
		 ZBUS_OBSERVERS(thingset_sub), /* observers */
		 ZBUS_MSG_INIT(0)	       /* Initial value {0} */
);
ZBUS_SUBSCRIBER_DEFINE(thingset_sub, 4);
ZBUS_CHAN_DECLARE(sensor_data_chan);

static void thingset_sensor_start(void)
{
	int ret;
	const struct zbus_channel *chan;
	struct sensor_zmsg s_zmsg;

	while (!zbus_sub_wait(&thingset_sub, &chan, K_FOREVER)) {
		if (&sensor_data_chan == chan) {
			ret = zbus_chan_read(&sensor_data_chan, &s_zmsg, K_MSEC(500));

			if (ret < 0) {
				LOG_ERR("zbus read data failed");
				continue;
			}

			data_objects_sensor_data(&s_zmsg);
		}
	}
}

K_THREAD_DEFINE(thingset_sensor, 1024, thingset_sensor_start, NULL, NULL, NULL, PRIORITY, 0, 0);
