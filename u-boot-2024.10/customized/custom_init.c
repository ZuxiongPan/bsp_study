#include "custom_struct.h"
#include <fs.h>
#include <ext4fs.h>

static struct custom_struct custom;
static struct fstype_info customExt4Fs = 
{
		.fstype = FS_TYPE_EXT,
		.name = "ext4",
		.null_dev_desc_ok = false,
		.probe = ext4fs_probe,
		.close = ext4fs_close,
		.ls = ext4fs_ls,
		.exists = ext4fs_exists,
		.size = ext4fs_size,
		.read = ext4_read_file,
		.uuid = ext4fs_uuid,
};

const char fsDevice[] = "mmc";
const unsigned int deviceNumber = 0;
const unsigned int partNumber = 0;

int custom_struct_init(struct custom_struct* pCustom)
{
	int ret = 0;
	memset(pCustom, 0, sizeof(struct custom_struct));

	pCustom->pFstypeInfo = &customExt4Fs;
	pCustom->pFsBlockDescribe = blk_get_devnum_by_uclass_idname(fsDevice, deviceNumber);
	if(NULL == pCustom->pFsBlockDescribe)
	{
		printf("No device for iface [%s], dev [%d]\n", fsDevice, deviceNumber);
		return -ENODEV;
	}
	ret = blk_dselect_hwpart(pCustom->pFsBlockDescribe, partNumber);
	if((ret != 0) || (pCustom->pFsBlockDescribe->type == DEV_TYPE_UNKNOWN))
	{
		printf("Failed to select h/w partition, error [%d]\n", ret);
		printf("Bad device [%s] [%d:%d]\n", fsDevice, deviceNumber, partNumber);
		return -ENODEV;
	}
	if(blk_enabled())
	{
		if (pCustom->pFsBlockDescribe->uclass_id == UCLASS_MMC)
			part_init(pCustom->pFsBlockDescribe);
	}
	pCustom->pFsBlockDescribe->log2blksz = LOG2(pCustom->pFsBlockDescribe->blksz);
	part_get_info_whole_disk(pCustom->pFsBlockDescribe, &(pCustom->fsDiskPartition));
	
	ret = pCustom->pFstypeInfo->probe(pCustom->pFsBlockDescribe, &(pCustom->fsDiskPartition));
	if(!ret)
	{
		printf("%s probe failed\n", pCustom->pFstypeInfo->name);
		return -ENODEV;
	}
	
	return 0;
}