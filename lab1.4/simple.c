#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/utsname.h>

void get_time(void);
void getSystemInfo(void);

/* This function is called when the module is loaded. */
int simple_init(void) {
       	printk(KERN_INFO "Initialization\n");
	get_time();
	getSystemInfo();
       	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Termination\n");
}

void get_time(void) {
	printk("Uptime: %d seconds\n",jiffies_to_msecs(get_jiffies_64())/1000);
}

void getSystemInfo(void){
	struct new_utsname *info=utsname();
	printk("Sysname: %s\n",info->sysname);
	printk("Release: %s\n",info->release);
	printk("Nodename: %s\n",info->nodename);
	printk("Version: %s\n",info->version);
	printk("Machine: %s\n",info->machine);

}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
