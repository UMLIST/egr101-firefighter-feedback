/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 * 
 * 
 * modified by ebs27 for EGR101
 */

/** @file
 *  @brief Nordic Mesh light sample
 */
#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include <bluetooth/mesh/dk_prov.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/drivers/gpio.h>
#include "model_handler.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(chat, CONFIG_LOG_DEFAULT_LEVEL);

#define LED0_Node DT_ALIAS(led0)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_Node, gpios);
static bool message_flag;

// toggle the flag on/off to blink LED each time a mesh message is sent
void update_message_flag(void)
{
	if (message_flag){
		message_flag = false;
		gpio_pin_set_dt(&led0, 0);

	}
	else {
		message_flag = true;
		gpio_pin_set_dt(&led0, 1);
	}
}

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");


	err = bt_mesh_init(bt_mesh_dk_prov_init(), model_handler_init());
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
		
		k_sleep(K_MSEC(1000));
	}

	/* This will be a no-op if settings_load() loaded provisioning info */
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);

	printk("Mesh initialized\n");
}

int main(void)
{
	int err;

	printk("Initializing...\n");

	// Setup the LEDs and the Button

 	if (!gpio_is_ready_dt(&led0)) {
		return 0;
	}

	err = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
	if (err < 0) {
		return 0;
	}

	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}

	// every second, generate faux message, toggle LED0
	while(1)
	{

	k_sleep(K_SECONDS(1));

	err = generate_mesh_message();
	if (err) {
		printk("Generate Mesh Message Failed (err %d)\n", err);
		gpio_pin_set_dt(&led0, 0);
		message_flag = false;
		return 0;
	}

	else {
	update_message_flag();
	}

	}

	return 0;
}
