#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/usb.h>
#include <linux/tty_port.h>
#include <linux/usb/serial.h>
#include <linux/mutex.h>
#include "usb_to_ser.h"
#define CP_VENDOR   0x1a86
#define CP_PRODUCT  0x55d4

struct usb_test {
    struct usb_serial_port *port;
    struct usb_serial *serial;
    struct mutex lock;

    u8      bInterfaceNumber;
	bool    event_mode;
};
static int cp210x_write_reg_block(struct usb_serial_port *port, u8 req, void *buf, int bufsize)
{
    struct usb_test *cp2102 = usb_get_serial_port_data(port);
    return usb_control_msg_send(cp2102->serial->dev, 0, req,
            REQTYPE_HOST_TO_INTERFACE, 0,
            cp2102->bInterfaceNumber, buf, bufsize,
            USB_CTRL_SET_TIMEOUT, GFP_KERNEL);
}
static int cp210x_write_u32_reg(struct usb_serial_port *port, u8 req, u32 val)
{
    __le32 le32_val;

    le32_val = cpu_to_le32(val);

    return cp210x_write_reg_block(port, req, &le32_val, sizeof(le32_val));
}
static int cp2102_write_reg(struct usb_serial_port *port, uint8_t request, uint16_t value)
{
    struct usb_test *cp2102 = usb_get_serial_port_data(port);
    return usb_control_msg(cp2102->serial->dev, usb_sndctrlpipe(cp2102->serial->dev, 0),
     request, REQTYPE_HOST_TO_INTERFACE, value, cp2102->bInterfaceNumber, NULL, 0, USB_CTRL_SET_TIMEOUT);
}
static int  cp2102_open(struct tty_struct *tty, struct usb_serial_port *port)
{
	int result;
    struct usb_test *cp2102 = usb_get_serial_port_data(port);
	result = usb_serial_generic_open(tty, port);
	if (result)
		goto err_disable;
	return 0;
err_disable:
	cp2102_write_reg(port, IFC_ENABLE, UART_DISABLE);
	cp2102->event_mode = false;
	return result;
}
static int  cp2102_write(struct tty_struct *tty, struct usb_serial_port *port,const unsigned char *buf, int count)
{
    return usb_serial_generic_write(tty, port, buf, count);
}
static void cp2102_close(struct usb_serial_port *port)
{
    usb_serial_generic_close(port);
}
static int cp2102_port_probe(struct usb_serial_port *port)
{
    struct usb_host_interface *cur_altsetting;
    struct usb_test *cp2102 = kzalloc(sizeof(*cp2102), GFP_KERNEL);
    if(!cp2102)
        return -ENOMEM;
    usb_set_serial_port_data(port, cp2102);
    cp2102->port = port;
    cp2102->serial = port->serial;
    cur_altsetting = port->serial->interface->cur_altsetting;
    cp2102->bInterfaceNumber = cur_altsetting->desc.bInterfaceNumber;
    mutex_init(&cp2102->lock);
    cp210x_write_u32_reg(port, SET_BAUDRATE, 115200);
    cp210x_write_u32_reg(port, SET_LINE_CTL, 0x00); //1 stop bit, no parity, 8bit data
    pr_info("Start USB %d\n", cp2102->bInterfaceNumber);
    return 0;
}
static void cp2102_port_remove(struct usb_serial_port *port)
{
    struct usb_test *cp2102 = usb_get_serial_port_data(port);
    mutex_destroy(&cp2102->lock);
    kfree(cp2102);
    pr_info("Stop USB\n");
}

static struct usb_device_id cp210x_device_id[] = {
    { USB_DEVICE(CP_VENDOR, CP_PRODUCT) },
    { }
};

static struct usb_serial_driver mydrv = {
    .port_probe = cp2102_port_probe,
    .port_remove = cp2102_port_remove,
    .driver = {
        .name = "cp2102",
        .owner = THIS_MODULE,
    },
    .id_table           = cp210x_device_id,
    .num_ports		    = 1,
	.bulk_in_size		= 256,
	.bulk_out_size		= 256,
    .open               = cp2102_open,
    .write              = cp2102_write,
    .close              =cp2102_close,
};
static struct usb_serial_driver * const serial_drivers[] = {
    &mydrv, NULL
};

module_usb_serial_driver(serial_drivers, cp210x_device_id);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("USB to Serial Driver");
MODULE_VERSION("1.0");
