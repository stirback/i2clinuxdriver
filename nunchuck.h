#ifndef NUNCHUCK_H
#define NUNCHUCK_H

static int nunchuck_init(struct i2c_client *client, uint8_t ret);
static int nunchuck_read_data(struct i2c_client *client, uint8_t ret); 

#endif /* defined(NUNCHUCK_H) */