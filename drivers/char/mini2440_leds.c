#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>


#define DEVICE_NAME "leds"

static unsigned long led_table [] = {
	S3C2410_GPB(5),
	S3C2410_GPB(6),
	S3C2410_GPB(7),
	S3C2410_GPB(8),
};

static unsigned int led_cfg_table [] = {
	S3C2410_GPIO_OUTPUT,
	S3C2410_GPIO_OUTPUT,
	S3C2410_GPIO_OUTPUT,
	S3C2410_GPIO_OUTPUT,
};
//kong zhi led kai guan ,zai ying yong ceng zen me diao yong de zhe ge han shu?
static int sbc2440_leds_ioctl(
	struct inode *inode, //struct inode
	struct file *file,   //struct file
	unsigned int cmd, // unsigned int 
	unsigned long arg)// unsigned long
{
	switch(cmd) {
	case 0:
	case 1:
		if (arg > 4) {
			return -EINVAL;
		}
		s3c2410_gpio_setpin(led_table[arg], !cmd);
		return 0;
	default:
		return -EINVAL;
	}
}

static struct file_operations dev_fops = {//cao zuo jie gou ti 
	.owner	=	THIS_MODULE,
	.ioctl	=	sbc2440_leds_ioctl,
};

static struct miscdevice misc = {//zhu ce za xiang she bei ?在内核中用struct miscdevice表示miscdevice设备
	.minor = MISC_DYNAMIC_MINOR,//minor是这个混杂设备的次设备号，若由系统自动配置，则可以设置为MISC_DYNANIC_MINOR，
	.name = DEVICE_NAME,//name是设备名 
	.fops = &dev_fops,
};
/*对于用户空间接口来说，我们的实现函数只是给出了IO的值设置的，
但是ARM的IO管脚使用还是需要配置方向、上拉下拉.....才能正常使用的，
并且所有的硬件资源，都是受内核所支配的，
驱动程序必需向内核申请硬件资源才能对硬件进行操作。
另外还需要对设备进行注册，内核才知道你这个设备是什么东东，
用到哪些东西。这些操作，我们安排在init里实现！*/
static int __init dev_init(void)
{
	int ret;

	int i;
	
	for (i = 0; i < 4; i++) {
		s3c2410_gpio_cfgpin(led_table[i], led_cfg_table[i]);
		s3c2410_gpio_setpin(led_table[i], 0);
	}

	ret = misc_register(&misc);//zhu ce hun za she bei 

	printk (DEVICE_NAME"\tinitialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
  int i;

  for (i = 0; i < 4; i++) {
    gpio_free(led_table[i]);
  }
  misc_deregister(&misc);
  printk (DEVICE_NAME"\texit led misc driver\n");
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("airui Inc.");
