#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#define DEV_NAME "rdwr"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dinh Huu Nam");
MODULE_DESCRIPTION("Simple character driver");
MODULE_VERSION("1.0");
struct char_device {
    dev_t dev_num;
    struct class *mclass;
    struct device *mdevice;
    struct cdev *mcdev;
    char *buffer;
}foo;
int chr_open (struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "opened %s\n", filep->f_path.dentry->d_name.name);
    return 0;
}
int chr_close (struct inode *inodep, struct file *filep)
{
    return 0;
}
ssize_t chr_read (struct file *filep, char __user *user_buff, size_t size, loff_t *offset)
{
    size_t to_read;
    to_read = (size > strlen(foo.buffer) - *offset) ? (strlen(foo.buffer) - *offset) : size;
    if(copy_to_user(user_buff, foo.buffer, to_read))
    {
        printk(KERN_ERR "Read failed\n");
        return -1;
    }
    *offset += to_read;
    return to_read;
}
ssize_t chr_write (struct file *filep, const char __user *user_buff, size_t size, loff_t *offset)
{
    if(copy_from_user(foo.buffer, user_buff, size))
    {
        printk(KERN_ERR "Write failed\n");
        return -1;
    }
    return size;
}
struct file_operations fops = 
{
    .owner          = THIS_MODULE,
    .open           = chr_open,
    .release        = chr_close,
    .read           = chr_read,
    .write          = chr_write
};
static int char_init(void)
{
    if(alloc_chrdev_region(&foo.dev_num, 0, 1, DEV_NAME) < 0)
    {
        printk(KERN_ERR "Device number alocation failed\n");
        return -1;
    }
    foo.mclass = class_create(THIS_MODULE, DEV_NAME);
    if(!foo.mclass)
    {
        printk(KERN_ERR "Class creation failed\n");
        goto rm_dev_num;
    }
    foo.mcdev = cdev_alloc();
    cdev_init(foo.mcdev, &fops);
    foo.mcdev->owner = THIS_MODULE;
    foo.mcdev->dev = foo.dev_num;
    foo.mcdev->ops = &fops;
    if(cdev_add(foo.mcdev, foo.dev_num, 1) < 0)
    {
        printk(KERN_ERR "Cdev add failure\n");
        goto rm_class;
    }
    foo.mdevice = device_create(foo.mclass, NULL, foo.dev_num, NULL, DEV_NAME);
    if(!foo.mdevice)
    {
        printk(KERN_ERR "Device create failure\n");
        goto rm_cdev;
    }
    foo.buffer = (char *)kmalloc(4096, GFP_KERNEL);
    if(!foo.buffer)
    {
        printk(KERN_ERR "Memory allocation failed\n");
        goto rm_device;
    }
    printk(KERN_INFO "Init Success\n");
    return 0;
rm_device:
    device_destroy(foo.mclass, foo.dev_num);
rm_cdev:
    cdev_del(foo.mcdev);
rm_class:
    class_destroy(foo.mclass);
rm_dev_num:
    unregister_chrdev_region(foo.dev_num, 1);
    return -1;
}
static void char_exit(void)
{
    kfree(foo.buffer);
    device_destroy(foo.mclass, foo.dev_num);
    cdev_del(foo.mcdev);
    class_destroy(foo.mclass);
    unregister_chrdev_region(foo.dev_num, 1);
}

module_init(char_init);
module_exit(char_exit);
