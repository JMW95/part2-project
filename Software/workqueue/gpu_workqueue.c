#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <asm/io.h>

#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/uaccess.h>

#define HW_REGS_BASE 0xc0000000
#define HW_REGS_SPAN 0x0c000000 // Actually 0x3c000000, but this doesn't work..
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define ALT_FPGASLVS_OFST  0xc0000000

#define LW_REGS_BASE 0xff200000
#define LW_REGS_SPAN 0x00200000
#define LW_REGS_MASK ( LW_REGS_SPAN - 1 )
#define ALT_LWFPGASLVS_OFST 0xff200000

#define WQ_0_BASE 0x200000
#define WQ_0_CSR 0x1000

#define WQ_BASE_JUMP 0x10000
#define WQ_CSR_JUMP 0x1000

#define BUFFERSIZE  512

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jamie Wood");
MODULE_DESCRIPTION("IRQ Workqueue driver");

#define NUMQUEUES 8
volatile unsigned int *bases[NUMQUEUES];
volatile unsigned int *csrs[NUMQUEUES];
static wait_queue_head_t wait_queues[NUMQUEUES];

static volatile unsigned int rd[NUMQUEUES];
static volatile unsigned int wr[NUMQUEUES];
static unsigned int *buffers[NUMQUEUES];

void *lwbase;
void *hwbase;

static DECLARE_WAIT_QUEUE_HEAD (done_queue);

static struct mutex locks[NUMQUEUES];

static unsigned int done[NUMQUEUES];

static unsigned int queue_free_space(int qn){
    int space;
    space = (rd[qn] - wr[qn]) - 1;
    if (space < 0){
        space += BUFFERSIZE;
    }
    return space;
}

static unsigned int queue_used_space(int qn){
    int space;
    space = (wr[qn] - rd[qn]);
    if (space < 0){
        space += BUFFERSIZE;
    }
    return space;
}

static void do_a_copy(int wqnum){
    mutex_lock(&locks[wqnum]);
    
    int i;
    int len = queue_used_space(wqnum);
    int space = 127 - csrs[wqnum][0]; // How much space is left in the FIFO
    if (len > space) len = space;
    //for(i=0; i<len; i++){
    while(len--){
        bases[wqnum][0] = buffers[wqnum][rd[wqnum]];
        rd[wqnum] = (rd[wqnum] + 1) & (BUFFERSIZE-1);
    }
    
    wake_up_all(&wait_queues[wqnum]); // Unblock any waiting processes
    
    mutex_unlock(&locks[wqnum]);
}

irq_handler_t copy_thread_func(int irq, void *dev_id, struct pt_regs *regs){
    int wqnum;
    wqnum = irq - 73;
    
    /*if(queue_used_space(wqnum) == 0){
        done[wqnum] = 1;
        wake_up_all(&done_queue);
        //printk(KERN_ALERT "e %d\n", wqnum);
    }else{*/
        do_a_copy(wqnum);
    //}
    
    return (irq_handler_t) IRQ_HANDLED;
}

irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs){
    int wqnum = irq - 73;
    
    // Clear the 'empty' event
    iowrite32(0xff, csrs[wqnum]+2);
    
    return (irq_handler_t)IRQ_WAKE_THREAD;
}

static ssize_t write(struct file *file, const char __user *user, size_t size,loff_t*o){
    int wqnum, nbytes, len, type, i, copylen, res, entrynum;
    unsigned int tmp, tw, tr;
    static unsigned int buf[1024]; // max data input is a 4k buffer
    static unsigned int *data;
    //printk(KERN_ALERT "%p %p %d\n", data, user, size);
    //if((res = copy_from_user(buf, user, size)) != 0){
    //    printk(KERN_ALERT "CFU FAILED %d b remain\n", res);
    //}
    
    //data = &buf[0];
    data = &user[0];
    
    for(entrynum=0; entrynum < size / 32; entrynum++){    
        wqnum = data[0] >> 24;
        nbytes = (data[0] >> 16) & 0xff;
        len = ((nbytes + 3) / 4) + 1; // add one for the header entry
        type = data[0] & 0xff;
        
        // replace the first entry with the header
        data[0] = (type << 8) | nbytes;
        
        copylen = 0;
        
        if(queue_free_space(wqnum) < len){
            //printk(KERN_ALERT "BL #%d, %d, %d (%d) (%d, %d)\n", wqnum, queue_free_space(wqnum), len, csrs[wqnum][0], wr[wqnum], rd[wqnum]);
            //tmp = csrs[wqnum][0];
            //tw = wr[wqnum];
            //tr = rd[wqnum];
            //asm("" ::: "memory");
            
            res = wait_event_interruptible_timeout(wait_queues[wqnum], queue_free_space(wqnum) >= len, HZ/10); // Block until there's room in the buffer
            //res = wait_event_interruptible(wait_queues[wqnum], queue_free_space(wqnum) >= len); // Block until there's room in the buffer
            
            //if(res <= 1){
            //    printk(KERN_ALERT "WAKE %d\n", res);
            //    printk(KERN_ALERT "PRE  #%d, %d, %d (%d) (%d, %d)\n", wqnum, queue_free_space(wqnum), len, tmp, tw, tr);
            //    printk(KERN_ALERT "POST #%d, %d, %d (%d) (%d, %d)\n", wqnum, queue_free_space(wqnum), len, csrs[wqnum][0], wr[wqnum], rd[wqnum]);
            //}
            
            //printk(KERN_ALERT "wake #%d, %d (%d) (%d, %d)\n", wqnum, queue_free_space(wqnum), csrs[wqnum][0], wr[wqnum], rd[wqnum]);
        }
        
        for(i=copylen; i<len; i++){
            buffers[wqnum][wr[wqnum]] = data[i];
            wr[wqnum] = (wr[wqnum] + 1) & (BUFFERSIZE-1);
        }
        //if(copylen < len){
            //printk(KERN_ALERT "copied %d bytes\n", (len - copylen));
        //}
        
        //printk(KERN_ALERT "POST s %d, w %d, r %d, f %d\n", len, wr[wqnum], rd[wqnum], csrs[wqnum][0]);
        
        data += 8;
        
        done[wqnum] = 0;
    }
    
    for(i=0; i<NUMQUEUES; i++){
        // Copy some data to get it started
        do_a_copy(i);
    }
    
    return size;
}

static bool allqueuesempty(void){
    int i;
    for(i=0; i<NUMQUEUES; i++){
        if(done[i] == 0){
            return false;
        }
    }
    return true;
}

static ssize_t read(struct file *file, char __user *user, size_t size,loff_t*o){
    if(!allqueuesempty()){
        printk(KERN_ALERT "sl\n");
        wait_event_interruptible(done_queue, allqueuesempty()); // Block until all queues are empty
        printk(KERN_ALERT "wa\n");
    }
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

static const struct file_operations workqueue_fops = {
    .owner   = THIS_MODULE,
    .open    = open,
    .release = close,
    .write   = write,
    .read    = read,
    .llseek  = default_llseek,
};

static struct miscdevice workqueue_dev = {
    .minor  = MISC_DYNAMIC_MINOR,
    .name   = "gpu_workqueue",
    .fops   = &workqueue_fops,
};

static int __init init_workqueue(void)
{
    int i;
    char name[20];
    
    // get the virtual addr that maps to the Avalon bridge
    lwbase = ioremap_nocache(LW_REGS_BASE, LW_REGS_SPAN);
    hwbase = ioremap_nocache(HW_REGS_BASE, HW_REGS_SPAN);
    
    printk(KERN_ALERT "lw: %p, hw: %p\n", lwbase, hwbase);
    
    for(i=0;i<NUMQUEUES;i++){
        
        bases[i] = hwbase + (( unsigned long)(ALT_FPGASLVS_OFST + WQ_0_BASE + (WQ_BASE_JUMP*i)) & (unsigned long) (HW_REGS_MASK) );
        csrs[i] = lwbase + (( unsigned long)(ALT_LWFPGASLVS_OFST + WQ_0_CSR + (WQ_CSR_JUMP*i)) & (unsigned long) (LW_REGS_MASK) );
        
        printk(KERN_ALERT "bases[%d] = %p\n", i, bases[i]);
        
        init_waitqueue_head(&wait_queues[i]);
        mutex_init(&locks[i]);
        
        /*printk(KERN_ALERT "fl: %x\n", (csrs[i]+0)[0]);
        printk(KERN_ALERT "is: %x\n", (csrs[i]+1)[0]);
        printk(KERN_ALERT "ev: %x\n", (csrs[i]+2)[0]);
        printk(KERN_ALERT "ie: %x\n", (csrs[i]+3)[0]);
        printk(KERN_ALERT "af: %x\n", (csrs[i]+4)[0]);
        printk(KERN_ALERT "ae: %x\n", (csrs[i]+5)[0]);*/
        
        // Enable the 'empty' interrupt
        iowrite32(0x2,csrs[i]+3);
        
        /*printk(KERN_ALERT "is: %x\n", (csrs[i]+1)[0]);
        printk(KERN_ALERT "ev: %x\n", (csrs[i]+2)[0]);
        printk(KERN_ALERT "ie: %x\n", (csrs[i]+3)[0]);*/
        
        // Allocate 4096 bytes for the buffer
        buffers[i] = (unsigned int *)kmalloc(BUFFERSIZE*sizeof(unsigned int), __GFP_WAIT | __GFP_IO | __GFP_FS);
        
        // Register the interrupt handler
        snprintf(name, sizeof(name), "workqueue_%d", i);
        
        if(request_threaded_irq(73+i, (irq_handler_t)irq_handler,
                   (irq_handler_t) copy_thread_func,
                   IRQF_SHARED, "workqueue",
                   (void *)(irq_handler)) != 0){
            return 1;
        }
    }
    
    return misc_register(&workqueue_dev);
}

static void __exit exit_workqueue(void)
{
    int i;
    for(i=0; i<NUMQUEUES; i++){
        free_irq(73+i, (void*) irq_handler);
        kfree(buffers[i]);
    }
    
    iounmap(lwbase);
    iounmap(hwbase);
    
    misc_deregister(&workqueue_dev);
}

module_init(init_workqueue);
module_exit(exit_workqueue);