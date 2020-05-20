/* See LICENSE file for license and copyright information */

#ifndef COOL_MODULE_H
#define COOL_MODULE_H

#include <stddef.h>

#define COOL_DEVICE_NAME "cool"
#define COOL_DEVICE_PATH "/dev/" COOL_DEVICE_NAME

#define COOL_IOCTL_MAGIC_NUMBER (long)0xc31

#define COOL_IOCTL_CMD_BTBF _IOR(COOL_IOCTL_MAGIC_NUMBER, 1, size_t)

#endif // COOL_MODULE_H
