#include <linux/module.h>	// 内核模块都需要使用的头文件
#include <linux/kernel.h>	// 包含 printk 函数声明
#include <linux/init.h>		// __init 和 __exit 宏定义
#include <linux/rbtree.h>
#include <linux/string.h>

struct TestStruct
{
	int data1;
	char buf1[16];
	struct rb_node node;
	int data2;
	char buf2[16];
};

static int __init pzxtest_init(void)
{
	struct TestStruct myStruct;
	memset((void*)&myStruct, 1, sizeof(myStruct));
	struct TestStruct* pMyStruct = &myStruct;
	struct TestStruct* pContainerGet = container_of(&(myStruct.node), struct TestStruct, node);
	printk("[%s]-%d: Hello World!\n", __FUNCTION__, __LINE__);
	printk("pMyStruct %x, pContainerGet %x\n", (unsigned int)pMyStruct, (unsigned int)pContainerGet);
	
	return 0;
}

static void __exit pzxtest_exit(void)
{
	printk("[%s]-%d: Goodbye Linux\n", __FUNCTION__, __LINE__);
	
	return ;
}

module_init(pzxtest_init);
module_exit(pzxtest_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pzx");
MODULE_DESCRIPTION("this is a test code");
MODULE_VERSION("1.0");