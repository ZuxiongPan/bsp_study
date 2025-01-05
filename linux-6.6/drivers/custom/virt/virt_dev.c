#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <uk_comm/comm_ioctl.h>

#define DEVNAME "virtdev"
#define DRVNAME "virtdrv"
#define CLASSNAME "virtclass"
static struct class *devClass = NULL;
static struct device *drvDevice = NULL;
static struct cdev virtDev;
dev_t devNum;

extern void debug_superblocks(void);

static long virtdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	
    switch (cmd)
	{
		case VIRT_IOCTL_GETSBINFO:
			debug_superblocks();
			break;
		default:
			pr_warn(DRVNAME ": Unknown IOCTL command\n");
			ret = -EINVAL;
			break;
    }

    return ret;
}

static struct file_operations virtdevFops =
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = virtdev_ioctl,
};

static int __init drv_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&devNum, 0, 1, DEVNAME);
    if (ret)
	{
        pr_err(DRVNAME ": Failed to allocate device number\n");
        return ret;
    }

    cdev_init(&virtDev, &virtdevFops);
    virtDev.owner = THIS_MODULE;
    ret = cdev_add(&virtDev, devNum, 1);
    if (ret)
	{
        pr_err(DRVNAME ": Failed to add cdev\n");
        unregister_chrdev_region(devNum, 1);
        return ret;
    }

    devClass = class_create(CLASSNAME);
    if (IS_ERR(devClass))
	{
        pr_err(DRVNAME ": Failed to create device class\n");
        cdev_del(&virtDev);
        unregister_chrdev_region(devNum, 1);
        return PTR_ERR(devClass);
    }

    drvDevice = device_create(devClass, NULL, devNum, NULL, DEVNAME);
    if (IS_ERR(drvDevice))
	{
        pr_err(DRVNAME ": Failed to create device\n");
        class_destroy(devClass);
        cdev_del(&virtDev);
        unregister_chrdev_region(devNum, 1);
        return PTR_ERR(drvDevice);
    }

    pr_info(DRVNAME ": Driver initialized successfully\n");
    return 0;
}

static void __exit drv_exit(void)
{
    device_destroy(devClass, devNum);
    class_destroy(devClass);
    cdev_del(&virtDev);
    unregister_chrdev_region(devNum, 1);
    pr_info(DRVNAME ": Driver exited\n");
}

module_init(drv_init);
module_exit(drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pzx + GPT");
