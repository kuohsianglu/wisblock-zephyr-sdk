/*
 * Copyright (c) 2025 Kenneth Lu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/mgmt/mcumgr/mgmt/mgmt.h>
#include <zephyr/mgmt/mcumgr/mgmt/callbacks.h>
#include <zephyr/sys/reboot.h>
#ifdef CONFIG_USB_DEVICE_STACK
#include <usb_work_q.h>
#endif

static struct mgmt_callback img_status_hook;
static struct k_work_delayable reboot_work;

static void reboot_work_handler(struct k_work *item)
{
	ARG_UNUSED(item);

	sys_reboot(SYS_REBOOT_WARM);
}

static void reboot_schedule(void)
{
	/*
	 * Reboot with a delay,
	 * so there is some time to send the status to the host
	 */
#ifdef CONFIG_USB_DEVICE_STACK
	k_work_schedule_for_queue(&USB_WORK_Q, &reboot_work, K_MSEC(500));
#else
	k_work_schedule_for_queue(&k_sys_work_q, &reboot_work, K_MSEC(500));
#endif
}

static enum mgmt_cb_return img_status_handler(uint32_t event, enum mgmt_cb_return prev_status,
					      int32_t *rc, uint16_t *group, bool *abort_more,
					      void *data, size_t data_size)
{
	if (event == MGMT_EVT_OP_IMG_MGMT_DFU_PENDING) {
		/* Rebooting when DFU operation has finished being transferred */
		reboot_schedule();
	}

	return MGMT_CB_OK;
}

void safeudate_mcumgr_init(void)
{
	img_status_hook.callback = img_status_handler;
	img_status_hook.event_id = MGMT_EVT_OP_IMG_MGMT_DFU_PENDING;
	mgmt_callback_register(&img_status_hook);
	k_work_init_delayable(&reboot_work, reboot_work_handler);
}
