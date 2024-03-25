#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/serdev.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/container_of.h>
struct uart_dev {
    struct serdev_device *serdev;
    struct device dev;
};
static int uart_recv(struct serdev_device *serdev, const uint8_t *buff, size_t size)
{
    uint8_t *buffer;
    buffer = kzalloc(1024, GFP_KERNEL);
    if(!buffer)
    {
        pr_err("Kzalloc failed\n");
        return -1;
    }
    memset(buffer, 0, 1024);
    memcpy(buffer, buff, size);
    pr_info("UART recv: %s\n", buffer);
    return size;
}
static struct serdev_device_ops sops = {
    .receive_buf = uart_recv,
};
static int uart_probe(struct serdev_device *serdev)
{
    struct uart_dev *cp2102 = NULL;
    cp2102 = devm_kzalloc(&serdev->dev, sizeof(*cp2102), GFP_KERNEL);
    if(!cp2102)
    {
        pr_err("Allocation failed\n");
        return -1;
    }
    cp2102->serdev = serdev;
    cp2102->dev = serdev->dev;
    serdev_device_set_drvdata(serdev, cp2102);
    serdev_device_set_client_ops(cp2102->serdev, &sops);
    if(serdev_device_open(cp2102->serdev) != 0)
    {
        pr_err("Cannot open serial device\n");
        return -1;
    }
    serdev_device_set_baudrate(cp2102->serdev, 9600);
    serdev_device_set_flow_control(cp2102->serdev, false);
    serdev_device_set_parity(cp2102->serdev, SERDEV_PARITY_NONE);

    serdev_device_write_buf(cp2102->serdev, "Hello World!\n", strlen("Hello World!\n"));
    pr_info("%s, %d\n", __func__, __LINE__);
    return 0;
}
static void uart_remove(struct serdev_device *serdev)
{
    struct uart_dev *cp2102 = serdev_device_get_drvdata(serdev);
    if(!cp2102)
    {
        pr_err("Get private data failure\n");
    }
    else
    {
        serdev_device_write_buf(cp2102->serdev, "Good Bye my fen\n", strlen("Good Bye my fen\n"));
        serdev_device_close(cp2102->serdev);
    }
    pr_info("%s, %d\n", __func__, __LINE__);
}

static const struct of_device_id uart_of_match_id[] = {
    { .compatible = "uart-cp2102,nam", },
    { }
};
MODULE_DEVICE_TABLE(of, uart_of_match_id);
static struct serdev_device_driver uart_driver = {
    .probe = uart_probe,
    .remove = uart_remove,
    .driver = {
        .name = "cp2102",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(uart_of_match_id)
    },
};
module_serdev_device_driver(uart_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DinhNam <20021163@vnu.edu.vn>");
MODULE_DESCRIPTION("Uart driver");
MODULE_VERSION("1.0");
