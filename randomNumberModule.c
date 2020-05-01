#include <linux/module.h> // Core header for loading LKMs
#include <linux/random.h> // get_random_bytes
#include <linux/init.h>   // __init & __exit
#include <linux/kernel.h> // Contain types, functions 
#include <linux/types.h>  // dev_t (major & minor number)
#include <linux/fs.h>     // Header for Linux file system support
#include <linux/uaccess.h>// Required for the copy_to_user funcition
#include <linux/device.h>

#define DEVICE_NAME "RandomNumberDevice" // Character device name
#define CLASS_NAME "RND"      // Character device driver
#define MAX 1000

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thanh Vo - Trang Nguyen - Phuc Tran");
MODULE_DESCRIPTION("Random number");
MODULE_VERSION("1.0");

static int majorNumber;
static int timesOfOpens = 0;
static struct class* RNClass = NULL;
static struct device* RNDevice = NULL;

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char*, size_t, loff_t*);

static struct file_operations fops = {
	.open = device_open,
	.release = device_release,
	.read = device_read,
};


static int __init RandomNumber_Init(void) {
	// Dynamically allocate a major number for device file
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0) {
		printk(KERN_ALERT "RandomNumber: failed to register a major number\n");
		return majorNumber;
	}

	printk(KERN_INFO "RandomNumber: registered successfully with major number %d\n", majorNumber);

		
	// Register the device class
	RNClass = class_create(THIS_MODULE, CLASS_NAME);
	
	if (IS_ERR(RNClass)) {
		// clean up if there's error
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		// return an error on a pointer
		return PTR_ERR(RNClass);
	}

	printk(KERN_INFO "RandomNumber: device class registered successfully\n");

	// Register the device driver
	RNDevice = device_create(RNClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(RNDevice)) {
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device driver\n");
		return PTR_ERR(RNDevice);
	}
	printk(KERN_INFO "RandomNumber: device class created correctly\n");
	return 0;
}


static void __exit RandomNumber_Exit(void) {
	// remove the device
	device_destroy(RNClass, MKDEV(majorNumber, 0)); 
	// unregister the device class
	class_unregister(RNClass);
	// remove the device class
	class_destroy(RNClass);
	// unregister the major number
	unregister_chrdev(majorNumber, DEVICE_NAME);

	printk(KERN_INFO "RandomNumber: Random number generator unregistered");
}

static int device_open(struct inode *inodep, struct file *filep) {
	timesOfOpens++;
	printk(KERN_INFO "RandomNumber: Device has been opened %d time(s)\n", timesOfOpens);
	return 0;
}

static int device_release(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "RandomNumber: Device successfully closed\n");
	return 0;
}

static ssize_t device_read(struct file *filep, char* usr_space, size_t len, loff_t* offset) {
	int rand;
	int error_cnt;
	// get_random_bytes has the format (void *buffer, int nbytes) and return random number  into &buffer
	get_random_bytes(&rand, sizeof(rand)); 
	rand %= MAX;

	// copy_to_user has the format (*to, *from, size) and returns 0 on success
	error_cnt = copy_to_user(usr_space, &rand, sizeof(rand));

	if (error_cnt == 0) {
		printk(KERN_INFO "RandomNumber: Sent random number to the user successfull\n");
		return 0;
	}
	else{
		printk(KERN_INFO "RandomNumber: Failed to send random number to the user!\n");
		return -EFAULT;

	}
}

module_init(RandomNumber_Init);
module_exit(RandomNumber_Exit);