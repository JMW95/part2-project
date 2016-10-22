#ifndef MEM_H
#define MEM_H


#define WQ_0_BASE 0x200000
#define WQ_1_BASE 0x201000
#define PIXELSTREAM_BASE 0x0
#define PALETTE_BASE 0x200

#define HW_REGS_BASE 0xc0000000
#define HW_REGS_SPAN 0x04000000
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define ALT_FPGASLVS_OFST  0xc0000000

#define LW_REGS_BASE 0xfc000000
#define LW_REGS_SPAN 0x04000000
#define LW_REGS_MASK ( LW_REGS_SPAN - 1 )
#define ALT_LWFPGASLVS_OFST 0xff200000

void *hw_init();
void *lw_init();


#endif /* MEM_H */
