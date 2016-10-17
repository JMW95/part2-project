#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/sched.h>
#include <linux/wait.h>

#define HW_REGS_BASE 0xc0000000
#define HW_REGS_SPAN 0x04000000
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define ALT_FPGASLVS_OFST  0xc0000000

#define LW_REGS_BASE 0xfc000000
#define LW_REGS_SPAN 0x04000000
#define LW_REGS_MASK ( LW_REGS_SPAN - 1 )
#define ALT_LWFPGASLVS_OFST 0xff200000

#define VSYNC_PIO_BASE   0x100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jamie Wood");
MODULE_DESCRIPTION("PixelStream VSync interrupt proxy");

volatile int *pio_base;
bool hasInterruptHappened = false;
static DECLARE_WAIT_QUEUE_HEAD (my_queue);

irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs){
    wake_up_interruptible(&my_queue); // Unblock any waiting processes

    hasInterruptHappened = true;

    // Clear the edgecapture register (clears current interrupt)
    iowrite32(0x1, pio_base+3);

    return (irq_handler_t) IRQ_HANDLED;
}

static ssize_t read(struct file *file, char __user *user, size_t size,loff_t*o){
    if(!hasInterruptHappened){
        wait_event_interruptible(my_queue, hasInterruptHappened); // Block until we get a vsync
    }
    hasInterruptHappened = false;
    return 1;
}

static int open(struct inode *inode, struct file *file){
    // Do nothing!
    return 0;
}

static int close(struct inode *inode, struct file *file){
    // Do nothing!
    return 0;
}

static const struct file_operations vsync_fops = {
    .owner   = THIS_MODULE,
    .open    = open,
    .release = close,
    //.write   = write,
    .read    = read,
    .llseek  = default_llseek,
};

static struct miscdevice vsync_dev = {
    .minor  = MISC_DYNAMIC_MINOR,
    .name   = "pixelstream_vsync",
    .fops   = &vsync_fops,
};

static int __init init_vsync_handler(void)
{
    // get the virtual addr that maps to the Avalon bridge
    void *vbase = ioremap_nocache(LW_REGS_BASE, LW_REGS_SPAN);
    pio_base = vbase + (( unsigned long)(ALT_LWFPGASLVS_OFST + VSYNC_PIO_BASE) & (unsigned long) (LW_REGS_MASK) );
    
    // Clear the PIO edgecapture register (clear any pending interrupt)
    iowrite32(0x1,pio_base+3);
    // Enable IRQ generation for the 4 buttons 
    iowrite32(0x1,pio_base+2);
    
    // Register the interrupt handler.
    if(request_irq(72, (irq_handler_t)irq_handler,
               IRQF_SHARED, "pixelstream_vsync",
               (void *)(irq_handler)) != 0){
        return 1;
    }
    
    return misc_register(&vsync_dev);
}

static void __exit exit_vsync_handler(void)
{
    // Turn off LEDs and de-register irq handler
    free_irq(72, (void*) irq_handler);
    misc_deregister(&vsync_dev);
}

module_init(init_vsync_handler);
module_exit(exit_vsync_handler);
