#include <linux/module.h>	// 内核模块都需要使用的头文件
#include <linux/kernel.h>	// 包含 printk 函数声明
#include <linux/init.h>		// __init 和 __exit 宏定义

static int __init pzxtest_init(void)
{
	printk("[%x]-%d: Hello Linux\n", &(pzxtest_init), __LINE__);
	
	return 0;
}

static void __exit pzxtest_exit(void)
{
	printk("[%x]-%d: Goodbye Linux\n", &(pzxtest_exit), __LINE__);
	
	return ;
}

module_init(pzxtest_init);
module_exit(pzxtest_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("pzx");
MODULE_DESCRIPTION("this is a test code");
MODULE_VERSION("1.0");