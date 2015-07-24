#include <stdio.h>
#include <stdlib.h>
#include <cell/gcm.h>

#include "../../../common/output.h"

// Writing on FIFO buffer
#define METHOD(offset, count) \
    (((count) << 18) | (offset))
#define PUT \
    *(uint32_t*)(LPAR_DMA_CONTROL_ADDR + 0x40)
#define COMMAND \
    ((uint32_t*)((uint32_t)ioAddress + PUT))

// TODO: Do the same without hardcoding the addresses
#define DEVICE_ADDR            0x40000000
#define DEVICE_SIZE            0x1000
#define LPAR_DMA_CONTROL_ADDR  0x40100000
#define LPAR_DMA_CONTROL_SIZE  0x1000
#define LPAR_DRIVER_INFO_ADDR  0x40200000
#define LPAR_DRIVER_INFO_SIZE  0x4000
#define LPAR_REPORTS_ADDR      0x40300000
#define LPAR_REPORTS_SIZE      0x10000

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // cellGcmInit
    const uint32_t cmdSize = 0x10000;
    const uint32_t ioSize = 1*1024*1024;
    const void *ioAddress = memalign(1*1024*1024, ioSize);
    cellGcmInit(cmdSize, ioSize, ioAddress);
    

    // Test: NV406E_SET_REFERENCE (0x0050)
    uint32_t* ref = (uint32_t*)(LPAR_DMA_CONTROL_ADDR + 0x48);
    *ref = 0;

    // Set REF to 0x12345678
    COMMAND[0] = METHOD(0x50, 1);
    COMMAND[1] = 0x12345678;
    PUT += 8;

    sys_timer_usleep(1000);
    fprintf(handler, "Reference: 0x%08X\n", *ref);


    // Test: NV406E_*_SEMAPHORE_* (0x0060, 0x0064, 0x0068, 0x006C)
    uint32_t* sem0x10 = (uint32_t*)(LPAR_REPORTS_ADDR + 0x10);
    uint32_t* sem0x20 = (uint32_t*)(LPAR_REPORTS_ADDR + 0x20);

    // Set both semaphores to 0x0
    COMMAND[0] = METHOD(0x60, 1);
    COMMAND[1] = 0x66616661;
    COMMAND[2] = METHOD(0x64, 1);
    COMMAND[3] = 0x10;
    COMMAND[4] = METHOD(0x6C, 1);
    COMMAND[5] = 0x0;
    COMMAND[6] = METHOD(0x64, 1);
    COMMAND[7] = 0x20;
    COMMAND[8] = METHOD(0x6C, 1);
    COMMAND[9] = 0x0;
    PUT += 40;

    sys_timer_usleep(1000);
    fprintf(handler, "Semaphore 1: 0x%08X\n", *sem0x10);
    fprintf(handler, "Semaphore 2: 0x%08X\n", *sem0x20);

    // Set 1st semaphore to 0x1
    COMMAND[0] = METHOD(0x64, 1);
    COMMAND[1] = 0x10;
    COMMAND[2] = METHOD(0x6C, 1);
    COMMAND[3] = 0x1;
    PUT += 16;

    sys_timer_usleep(1000);
    fprintf(handler, "Semaphore 1: 0x%08X\n", *sem0x10);

    // Set 2nd semaphore to 0xFFFFFFFF
    COMMAND[0] = METHOD(0x64, 1);
    COMMAND[1] = 0x20;
    COMMAND[2] = METHOD(0x6C, 1);
    COMMAND[3] = 0xFFFFFFFF;
    PUT += 16;

    sys_timer_usleep(1000);
    fprintf(handler, "Semaphore 2: 0x%08X\n", *sem0x20);

    // Make 1st semaphore wait for 0x2, and try to set the 2nd semaphore to 1
    COMMAND[0] = METHOD(0x64, 1);
    COMMAND[1] = 0x10;
    COMMAND[2] = METHOD(0x68, 1);
    COMMAND[3] = 0x2;
    COMMAND[4] = METHOD(0x64, 1);
    COMMAND[5] = 0x20;
    COMMAND[6] = METHOD(0x6C, 1);
    COMMAND[7] = 0x1;
    PUT += 32;

    // Set 2nd semaphore to 0x0 and the 1st one to 0x2 (manually)
    sys_timer_usleep(1000);
    *sem0x20 = 0;
    *sem0x10 = 2;

    // The previous commands should made the PFIFO puller wait. The 2nd semaphore should be 0x1
    sys_timer_usleep(1000);
    fprintf(handler, "Semaphore 1: 0x%08X\n", *sem0x10);
    fprintf(handler, "Semaphore 2: 0x%08X\n", *sem0x20);


    fclose(handler);
    return 0;
}
