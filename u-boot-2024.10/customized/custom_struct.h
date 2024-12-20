#ifndef __CUSTOM_STRUCT__
#define __CUSTOM_STRUCT__

#include <blk.h>
#include <part.h>

struct fstype_info {
	int fstype;
	char *name;
	bool null_dev_desc_ok;
	int (*probe)(struct blk_desc *fs_dev_desc,
		     struct disk_partition *fs_partition);
	int (*ls)(const char *dirname);
	int (*exists)(const char *filename);
	int (*size)(const char *filename, loff_t *size);
	int (*read)(const char *filename, void *buf, loff_t offset,
		    loff_t len, loff_t *actread);
	void (*close)(void);
	int (*uuid)(char *uuid_str);
};

struct custom_struct {
	struct fstype_info* pFstypeInfo;
	struct blk_desc* pFsBlockDescribe;
	struct disk_partition fsDiskPartition;
};

#endif