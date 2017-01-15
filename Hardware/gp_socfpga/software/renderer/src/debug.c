#include "debug.h"

void debug_write(unsigned char val){
	unsigned char *dbg = (unsigned char *)DEBUG_BASE;
	dbg[0] = val;
}
