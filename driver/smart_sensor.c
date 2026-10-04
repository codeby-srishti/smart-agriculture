#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DEVICE_NAME "smart_sensor"

static dev_t dev_number;
static struct cdev smart_cdev;
static struct class *smart_class;

/*
 * Simulated sensor data.
 *
 * In a real hardware implementation, this data would be obtained
 * from a physical sensor through an appropriate hardware interface
 * such as GPIO, I2C, SPI or ADC.
 */
static char sensor_data[] =
    "SOIL=45 TEMP=31.5 HUMIDITY=58\n";

/*
 * Called when the device is opened.
 */
static int smart_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Smart Sensor: Device opened\n");

    return 0;
}

/*
 * Called when data is read from /dev/smart_sensor.
 */
static ssize_t smart_read(struct file *file,
                          char __user *buffer,
                          size_t length,
                          loff_t *offset)
{
    size_t data_length;
    size_t bytes_to_copy;

    data_length = strlen(sensor_data);

    /* End of file */
    if (*offset >= data_length)
        return 0;

    /*
     * Copy only the amount of data that the
     * user-space buffer can hold.
     */
    bytes_to_copy = min(length,
                        data_length - (size_t)*offset);

    /*
     * Copy data from kernel space to user space.
     */
    if (copy_to_user(buffer,
                     sensor_data + *offset,
                     bytes_to_copy))
    {
        return -EFAULT;
    }

    *offset += bytes_to_copy;

    printk(KERN_INFO
           "Smart Sensor: Data sent to user space\n");

    return bytes_to_copy;
}

/*
 * Called when the device is closed.
 */
static int smart_release(struct inode *inode,
                         struct file *file)
{
    printk(KERN_INFO "Smart Sensor: Device closed\n");

    return 0;
}

/*
 * File operations supported by the character device.
 */
static const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = smart_open,
    .read = smart_read,
    .release = smart_release
};

/*
 * Driver initialization.
 */
static int __init smart_sensor_init(void)
{
    int result;

    printk(KERN_INFO "Smart Sensor: Driver loading\n");

    /*
     * Allocate a major and minor device number.
     */
    result = alloc_chrdev_region(&dev_number,
                                 0,
                                 1,
                                 DEVICE_NAME);

    if (result < 0)
    {
        printk(KERN_ERR
               "Smart Sensor: Failed to allocate device number\n");

        return result;
    }

    /*
     * Initialize the character device.
     */
    cdev_init(&smart_cdev, &fops);
    smart_cdev.owner = THIS_MODULE;

    /*
     * Add the character device to the kernel.
     */
    result = cdev_add(&smart_cdev,
                      dev_number,
                      1);

    if (result < 0)
    {
        printk(KERN_ERR
               "Smart Sensor: Failed to add character device\n");

        unregister_chrdev_region(dev_number, 1);

        return result;
    }

    /*
     * Create the device class.
     */
    smart_class = class_create(DEVICE_NAME);

    if (IS_ERR(smart_class))
    {
        printk(KERN_ERR
               "Smart Sensor: Failed to create device class\n");

        cdev_del(&smart_cdev);
        unregister_chrdev_region(dev_number, 1);

        return PTR_ERR(smart_class);
    }

    /*
     * Create /dev/smart_sensor.
     */
    if (IS_ERR(device_create(smart_class,
                             NULL,
                             dev_number,
                             NULL,
                             DEVICE_NAME)))
    {
        printk(KERN_ERR
               "Smart Sensor: Failed to create device\n");

        class_destroy(smart_class);
        cdev_del(&smart_cdev);
        unregister_chrdev_region(dev_number, 1);

        return -1;
    }

    printk(KERN_INFO
           "Smart Sensor: /dev/%s created\n",
           DEVICE_NAME);

    return 0;
}

/*
 * Driver cleanup when the module is removed.
 */
static void __exit smart_sensor_exit(void)
{
    device_destroy(smart_class, dev_number);

    class_destroy(smart_class);

    cdev_del(&smart_cdev);

    unregister_chrdev_region(dev_number, 1);

    printk(KERN_INFO
           "Smart Sensor: Driver unloaded\n");
}

module_init(smart_sensor_init);
module_exit(smart_sensor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Smart Agriculture Project");
MODULE_DESCRIPTION(
    "Linux Character Device Driver for Smart Agriculture Sensor"
);
MODULE_VERSION("1.0");
