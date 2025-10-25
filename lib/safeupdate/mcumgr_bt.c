/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2020 Prevas A/S
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/mgmt/mcumgr/transport/smp_bt.h>
#include <lorawan_ext/lorawan_ext.h>
#include <qgrid_cfg/qgrid_config.h>

#define LOG_LEVEL LOG_LEVEL_DBG
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(bt_mcumgr);

static struct k_work advertise_work;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, 0x84, 0xaa, 0x60, 0x74, 0x52, 0x8a, 0x8b, 0x86, 0xd3,
		      0x4c, 0xb7, 0x1d, 0x1d, 0xdc, 0x53, 0x8d),
};

static void advertise(struct k_work *work)
{
	int rc;

	bt_le_adv_stop();

	rc = bt_le_adv_start(BT_LE_ADV_CONN_ONE_TIME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (rc) {
		LOG_ERR("Advertising failed to start (rc %d)", rc);
		return;
	}

	LOG_INF("Advertising successfully started");
}

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection failed (err 0x%02x)", err);
	} else {
		LOG_INF("Connected");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason 0x%02x)", reason);
	k_work_submit(&advertise_work);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

void mcumgr_bt_ready(int err)
{
	if (err != 0) {
		LOG_ERR("Bluetooth failed to initialise: %d", err);
	} else {
#ifdef CONFIG_BT_DEVICE_NAME_DYNAMIC
		int ret;
		char dev_name[CONFIG_BT_DEVICE_NAME_MAX];

		if (IS_ENABLED(CONFIG_QUOGRID_CFG)) {
			uint8_t dev_eui[LORAWAN_EXT_EUI_SIZE] = {0};
			struct qgrid_cfg *qcfg = quogrid_get_cfg();

			lorawan_parse_eui(dev_eui, qcfg->deveui);
			bin2hex(dev_eui, LORAWAN_EXT_EUI_SIZE, dev_name, CONFIG_BT_DEVICE_NAME_MAX);
		} else {
			strncpy(dev_name, CONFIG_BT_DEVICE_NAME, CONFIG_BT_DEVICE_NAME_MAX);
		}

		ret = bt_set_name(dev_name);
		if (ret) {
			LOG_WRN("Failed to set device name (%d)", ret);
		}
#endif
		k_work_submit(&advertise_work);
	}
}

void mcumgr_bt_start_smp_adverts(void)
{
	k_work_init(&advertise_work, advertise);
}
