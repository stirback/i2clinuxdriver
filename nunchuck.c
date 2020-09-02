// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include "linux/delay.h"

static int nunchuck_init(struct i2c_client *client, uint8_t ret){
	uint8_t tx_buf[2] = {0xF0, 0x55};
	pr_alert("Init start\n");
	ret = i2c_master_send(client, tx_buf, sizeof(tx_buf));
	if (ret < 0) {
		pr_alert("failed to send sensor data\n");
		return ret;
	}
	msleep(1);
	tx_buf[0] = 0xFB;
	tx_buf[1] = 0x00;
	ret = i2c_master_send(client, tx_buf, sizeof(tx_buf));
	if (ret < 0) {
		pr_alert("failed to send sensor data\n");
		return ret;
	}
	pr_alert("Init worked\n");
	return ret;
}

static int nunchuck_read_data(struct i2c_client *client, uint8_t ret) {
	uint8_t i = 0;
	uint8_t rx_buf[6];
	uint8_t button_control = 0;
	pr_alert("Read start\n");
	for (i = 0; i < 2; ++i){
		msleep(10);
		ret = i2c_master_send(client, 0x00, 1);
		if (ret < 0) {
			return -1;
		}
		msleep(10);
		ret = i2c_master_recv(client, rx_buf, sizeof(rx_buf));
		if (ret < 0) {
			return -1;
		}
	}
	
	button_control = rx_buf[5];
	pr_alert("Button value is %u\n", button_control);
	
	if (button_control & 0x01){
		pr_alert("Z button is pressed");
	}

	if(button_control & 0x02){
		pr_alert("C button is pressed");
	}
	pr_alert("Read worked\n");
	return 0;
}

static int nunchuck_probe(struct i2c_client *client, const struct i2c_device_id *id){
	uint8_t ret = 0;
	ret = nunchuck_init(client, ret);
	if (ret < 0) {
		pr_alert("failed to send sensor data\n");
		return -1;
	}
	ret = nunchuck_read_data(client, ret);
	if (ret < 0) {
		pr_alert("failed to send/recv sensor data\n");
		return -1;
	}
	return 0;
}

static int nunchuck_remove(struct i2c_client *client){
	pr_alert("Bye\n");
	return 0;
}

static const struct i2c_device_id nunchuck_ids[] = {
	{"nintendo,nunchuck", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, nunchuck_ids);

static struct i2c_driver nunchuck_driver = {
	.driver.name	= "nunchuck",
	.id_table	= nunchuck_ids,
	.probe		= nunchuck_probe,
	.remove		= nunchuck_remove,
};

module_i2c_driver(nunchuck_driver);

MODULE_AUTHOR("Oleg Strbac <stirback@gmail.com>");
MODULE_DESCRIPTION("I2C nunchuck driver");
MODULE_LICENSE("GPL");

