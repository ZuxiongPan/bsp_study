#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/string.h>

#define BUFSIZE 128
#define MAGICNUMBER 0xaabbccdd
#define debug(const char* fmt, ...) \
	printk("[%s]-%d: " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

struct ListTest
{
	unsigned int value;
	char buffer[BUFSIZE];
	struct list_head node;
	unsigned int magic;
};

inline void set_listtest_struct(struct ListTest* pStruct, int val, const char* buf)
{
	pStruct->val = val;
	strncpy(pStruct->buffer, buf, BUFSIZE);
	pStruct->node = { NULL, NULL };
	pStruct->magic = MAGICNUMBER;
}

inline void print_listtest_struct(struct ListTest* pStruct)
{
	if (NULL == pStruct || MAGICNUMBER != pStruct->magic)
	{
		debug("this node is not exist or invalid\n");
		return ;
	}
	
	printk("value %x, buffer %s", pStruct->value, pStruct->buffer);
	
	return;
}

static struct ListTest list;

static int __init listtest_init(void)
{
	struct ListTest* position = NULL;
	set_listtest_struct(&list, 0x22334455, "this is header");
	INIT_LIST_HEAD(&(list.node));
	list_for_each(position, list)
	{
		printk("node param: ");
		print_listtest_struct(
	
	return 0;
}

static void __exit listtest_exit(void)
{
	
	return ;
}

module_init(listtest_init);
module_exit(listtest_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pzx");
MODULE_DESCRIPTION("this is a test for list_head in kernel");
MODULE_VERSION("v1.0");