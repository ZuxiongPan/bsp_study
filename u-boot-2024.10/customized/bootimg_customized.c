#include <stdio.h>
#include <fs.h>
#include <command.h>
#include "customized.h"

const char* loadKernel[64] = { "load", "mmc", "0:0", "0x60100000", "boot/uImage", NULL };
const char* loadDTB[64] = { "load", "mmc", "0:0", "0x60000000", "boot/vexpress-v2p-ca9.dtb", NULL };
const char* bootmInfo[64] = { "bootm", "0x60100000", "-", "0x60000000", NULL };

int customized_bootimg(void)
{
	printf("Start customized bootimg ...\n");
	
	
	printf("Start load Kernel to memory\n");
	int strNums = 0;
	while(loadKernel[strNums] != NULL)
		strNums++;
	do_load(NULL, 0, strNums, loadKernel, FS_TYPE_ANY);
	
	printf("Start load DTB to memory\n");
	strNums = 0;
	while(loadDTB[strNums] != NULL)
		strNums++;
	do_load(NULL, 0, strNums, loadDTB, FS_TYPE_ANY);
	
	printf("Start Booting Kernel ...\n");
	strNums = 0;
	while(bootmInfo[strNums] != NULL)
		strNums++;
	do_bootm(NULL, 0, strNums, bootmInfo);
	
	// never run here
	printf("Booting Kernel Error !!!\n");
	
	return 0;
}
