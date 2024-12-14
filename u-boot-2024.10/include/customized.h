#ifndef __CUSTOMIZED_H__
#define __CUSTOMIZED_H__

#include <linux/stddef.h>

#ifdef CONFIG_BOOTIMG_CUSTOMIZED
int customized_bootimg(void);
#endif

#endif