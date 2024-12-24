#include <stdio.h>
#include <fs.h>
#include <command.h>
#include <linux/delay.h>
#include <time.h>
#include "customized.h"

const char* loadKernel[64] = { "load", "mmc", "0:0", "0x60100000", "boot/uImage", NULL };
const char* loadDTB[64] = { "load", "mmc", "0:0", "0x60000000", "boot/vexpress-v2p-ca9.dtb", NULL };
const char* bootmInfo[64] = { "bootm", "0x60100000", "-", "0x60000000", NULL };
const int bootDelay = 2;

static int abortboot_by_key(int bootdelay)
{
	int abort = 0;
	unsigned long ts;

	printf("Hit any key to stop customized boot: %2d ", bootdelay);

	if (tstc()) {
		getchar();
		puts("\b\b\b 0");
		abort = 1;
	}

	while ((bootdelay > 0) && (!abort)) {
		--bootdelay;
		ts = get_timer(0);
		do {
			if (tstc()) {
				int key;

				abort  = 1;
				bootdelay = 0;
				key = getchar();
				break;
			}
			udelay(10000);
		} while (!abort && get_timer(ts) < 1000);

		printf("\b\b\b%2d ", bootdelay);
	}

	putc('\n');

	return abort;
}

static int bootimg(void)
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

int customized_bootimg(void)
{
	if(!abortboot_by_key(bootDelay))
	{
		bootimg();
		// never comes here
		return 0;
	}
	
	printf("run into boot cli\n");
	return 0;
}
