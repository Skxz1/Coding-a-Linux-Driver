#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sam");
MODULE_DESCRIPTION("Phase 2: Simple character device (open/release only");
MODULE_VERSION("0.1");


// dev_t is a kernel type that stores major + minor together.
// Think of it as: dev = (major, minor) packed into one value.

static dev_t mydev;

// cdev is the kernel object that ties a device number to file_operations
// Its the "character device" representation in the kernel

static struct cdev my_cdev;

// ----- File Operation Callbacks -----

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("mydevice: open()\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	pr_info("mydevice: release()\n");
	return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset);

// file operations is a table of function pointers.
// The VFS calls these when user-space interacts with /dev/mydeivce.

static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
};

// ----- Module Lifecycle -----


static int __init my_init(void)
{
	int ret;

	// Ask the kernel for a free major number and reserve 1 minor number.
	// After this, 'mydev' contains the allocated (major, minor=0).
	ret = alloc_chrdev_region(&mydev, 0, 1, "mydevice");
	if (ret < 0){
		pr_err("mydevice: alloc_chrdev_region failed: %d \n", ret);
		return ret;
	}

	pr_info("mydevice: allocated major=%d minor=%d\n",
		MAJOR(mydev), MINOR(mydev));

	// Initialise cdev with our file_operations table
	// Then add it to the kernel so it becomes "live".
	cdev_init(&my_cdev, &my_fops);

	ret = cdev_add(&my_cdev, mydev, 1);
	if (ret < 0){
		pr_err("mydevice: cdev_add failed: %d\n", ret);
		return ret;
	}

	pr_info("mydevice: module loaded \n");
	return 0;
}

static void __exit my_exit(void)
{
	cdev_del(&my_cdev);
	unregister_chrdev_region(mydev, 1);
	pr_info("mydevice: module unloaded\n");
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
        pr_info("mydevice: read(%zu)\n", len);
        return 0;       //EOF: tells cat theres nothing to read
}

module_init(my_init);
module_exit(my_exit);



