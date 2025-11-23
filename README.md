# WisBlock Zephyr SDK

This SDK designed to simplify developing with Rakwireless WisBlock modular hardware on the
[Zephyr](https://github.com/zephyrproject-rtos/zephyr) RTOS.

We encapsulate the complex driver configurations, hardware abstractions, and data flow management.
This lets you stop worrying about Device Tree files or writing boilerplate code.
Just enable the modules you want via Kconfig and start building your application.

## Why Use This SDK?
Our goal is to let you focus on your application logic, not the low-level hardware plumbing.

- ⚡️**Zero-Config Hardware**: There's no need to manually edit Device Tree (DTS) files or hunt down drivers. Simply add a
  line like `CONFIG_WISBLOCK_SENSOR_RAK1901=y` to your `prj.conf`, and the SDK automatically includes and configures the
  **RAK1901** temperature/humidity sensor and its underlying I2C drivers.
- 📬**Simplified Data Flow**: Sensor data is automatically read and broadcast asynchronously to the **ZBUS** message bus
  on the `sensor_data_chan` channel. Your application can simply subscribe to this channel to receive all sensor data, with no need to manage your own threads or polling mechanisms.
- 🚀**Built-in Secure DFU**: The `lib/safeupdate` library provides a ready-to-use solution for secure firmware updates. It integrates MCUboot and mcumgr, enabling secure Over-the-Air (OTA) DFU via Bluetooth LE out of the box.
- 📦**Application Ready**: We include the `thingset_lorawan` reference application to demonstrate how to take your sensor data (modeled with ThingSet) and transmit it over LoRaWAN.

## Supported WisBlock Modules

This SDK provides plug-and-play support for the following modules via Kconfig options:

| Module Type | Model    | Description                           | Kconfig Option                   |
|-------------|----------|---------------------------------------|----------------------------------|
| Core        | RAK4631  | nRF52840 + SX1262 Core Module         | (Target board rak4631)           |
| Sensor      | RAK1901  | Temperature & Humidity Sensor (SHTC3) | `CONFIG_WISBLOCK_SENSOR_RAK1901` |
| Sensor      | RAK1904  | 3-axis Acceleration Sensor (LIS3DH)   | `CONFIG_WISBLOCK_SENSOR_RAK1904` |
| IO          | RAK13007 | Relay Module (Omron G5LE-14)          | `CONFIG_WISBLOCK_IO_RAK13007`    |

## Quick Start
1. Initialize the Project
```
west init -m git@github.com:kuohsianglu/wisblock-zephyr-sdk.git wisblock-zephyrproject

cd wisblock-zephyrproject

west update
```

2. Build
    - RAK4631 + RAK1901(Temperature & Humidity Sensor)
    ```
    west build -p -b rak4631 rakwireless/thingset_lorawan/ -- -DOVERLAY_CONFIG='overlay-bt_mcumgr.conf overlay-lorawan.conf' -DCONFIG_WISBLOCK=y -DCONFIG_WISBLOCK_SENSOR_RAK1901=y
    ```

    - RAK4631 + RAK1904(3-axis Acceleration Sensor)
    ```
    west build -p -b rak4631 rakwireless/thingset_lorawan/ -- -DOVERLAY_CONFIG='overlay-bt_mcumgr.conf overlay-lorawan.conf' -DCONFIG_WISBLOCK=y -DCONFIG_WISBLOCK_SENSOR_RAK1904=y
    ```

3. Flashing
Connect your RAK4631 board via J-Link and run the following command to flash the firmware:
    ```bash
    west flash
    ```

    > **Note:** For detailed instructions on programming and debugging the RAK4631 (including hardware setup and alternative methods), please refer to the [Zephyr Official Guide: Programming and Debugging](https://docs.zephyrproject.org/latest/boards/rakwireless/rak4631/doc/index.html#programming-and-debugging).

