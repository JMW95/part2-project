#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "mem.h"

void *hw_init(){
    int fd;
    if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return NULL;
    }
    
    void *virtual_base;
    virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

    if(virtual_base == MAP_FAILED){
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return NULL;
    }
    
    return virtual_base;
}

void *lw_init(){
    int fd;
    if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
        printf("ERROR: could not open \"/dev/mem\"...\n");
        return NULL;
    }
    
    void *virtual_base;
    virtual_base = mmap(NULL, LW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, LW_REGS_BASE);

    if(virtual_base == MAP_FAILED){
        printf("ERROR: mmap() failed...\n");
        close(fd);
        return NULL;
    }
    
    return virtual_base;
}