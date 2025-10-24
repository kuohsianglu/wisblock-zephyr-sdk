#ifndef ZEPHYR_INCLUDE_BT_MCUMGR_LIB_H_
#define ZEPHYR_INCLUDE_BT_MCUMGR_LIB_H_

/**
 * @brief Callback for notifying that Bluetooth has been enabled.
 *
 * @param err zero on success or (negative) error code otherwise.
 */
void mcumgr_bt_ready(int err);

/**
 * @brief Start to advertise smp service.
 */
void mcumgr_bt_start_smp_adverts(void);

#endif /* ZEPHYR_INCLUDE_BT_MCUMGR_LIB_H_ */
