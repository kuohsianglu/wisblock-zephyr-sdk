/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <wisblock/wb_io.h>
#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wb_io);

#ifdef CONFIG_WISBLOCK_IO_RAK13007
static const struct gpio_dt_spec relay_ctrl = GPIO_DT_SPEC_GET(DT_ALIAS(relay0), gpios);

static const struct gpio_dt_spec relay_pwr = GPIO_DT_SPEC_GET(DT_ALIAS(relay1), gpios);
#endif

int wisblock_io_relay_onoff(void)
{
	int rc = 0;
#ifdef CONFIG_WISBLOCK_IO_RAK13007
	rc = gpio_pin_configure_dt(&relay_ctrl, GPIO_OUTPUT_HIGH);
	if (rc < 0) {
		LOG_ERR("relay active failed: %d", rc);
	}

	k_sleep(K_MSEC(200));

	rc = gpio_pin_configure_dt(&relay_ctrl, GPIO_OUTPUT_LOW);
	if (rc < 0) {
		LOG_ERR("relay inactive failed: %d", rc);
	}

	LOG_INF("RAK13007 Relay: set done!");
#endif
	return rc;
}

static int wisblock_io_init(void)
{
	int ret = 0;
#ifdef CONFIG_WISBLOCK_IO_RAK13007
	if (!gpio_is_ready_dt(&relay_ctrl) || !gpio_is_ready_dt(&relay_pwr)) {
		LOG_ERR("Relay gpio not ready.");
		return -1;
	}

	ret = gpio_pin_configure_dt(&relay_ctrl, GPIO_OUTPUT_LOW);
	if (ret < 0) {
		LOG_ERR("Relay control configure failed.");
		return ret;
	}

	ret = gpio_pin_configure_dt(&relay_pwr, GPIO_OUTPUT_HIGH);
	if (ret < 0) {
		LOG_ERR("Relay power configure failed.");
		return ret;
	}
#endif
	return ret;
}

SYS_INIT(wisblock_io_init, APPLICATION, WB_IO_INIT_PRIORITY_DEFAULT);
