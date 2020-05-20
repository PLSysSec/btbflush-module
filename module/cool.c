#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/version.h>
#include <asm/microcode.h>
#include "cool_header.h"


MODULE_AUTHOR("Daniel Moghimi");
MODULE_DESCRIPTION("COOL Kernel Module");
MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file) {
  return 0;
}

static int device_release(struct inode *inode, struct file *file) {
  return 0;
}



static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
  switch (ioctl_num) {
    case COOL_IOCTL_CMD_BTBF:
    {
        // native_wrmsrl((0x49), (unsigned long)(0x1));
        asm volatile("wrmsr;"
                    :
                    : "a" (0x1)
                    , "c" (0x49)
                    , "d" ((uint32_t)0));
        return 0;
    }

    default:

        return -1;
  }

  return 0;
}

static struct file_operations f_ops = {.unlocked_ioctl = device_ioctl,
                                       .open = device_open,
                                       .release = device_release};

static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = COOL_DEVICE_NAME,
    .fops = &f_ops,
    .mode = S_IRWXUGO,
};

int init_module(void) {
  int r;

  /* Register device */
  r = misc_register(&misc_dev);
  if (r != 0) {
    printk(KERN_ALERT "[cool-module] Failed registering device with %d\n", r);
    return 1;
  }

  printk(KERN_INFO "[cool-module] Loaded.\n");

  return 0;
}

void cleanup_module(void) {
  misc_deregister(&misc_dev);

  printk(KERN_INFO "[cool-module] Removed.\n");
}
