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

// Flip Status location
#define FLIP_STATUS(head)      *(uint32_t*)(LPAR_DRIVER_INFO_ADDR + 0x10C0 + 0x40 * head)

int main(void)
{
    FILE* handler = fopen("/app_home/" OUTPUT_FILE, "wb");

    // cellGcmInit
    const uint32_t cmdSize = 0x10000;
    const uint32_t ioSize = 1*1024*1024;
    const void *ioAddress = memalign(1*1024*1024, ioSize);
    cellGcmInit(cmdSize, ioSize, ioAddress);
    

    /**
     * Test: SCE_DRIVER_QUEUE (0xE940:0xE95C)
     */

    // Resetting status
    for (int head = 0; head < 8; head++) {
        FLIP_STATUS(head) = 0x00000000;
    }

    // Test for each head
    for (int head = 0; head < 8; head++) {
        fprintf(handler, "SCE_DRIVER_QUEUE [0x%04X]:\n", 0xE940 + 4*head);

        // Queuing one at a time (and resetting status)
        for (int parameter = 0; parameter < 32; parameter++) {
            COMMAND[0] = METHOD(0xE940 + 4 * head, 1);
            COMMAND[1] = parameter;
            PUT += 8;

            sys_timer_usleep(1000);
            fprintf(handler, "Flip Status [DRIVER_INFO @ 0x%04X]: 0x%08X\n", 0x10C0 + 0x40 * head, FLIP_STATUS(head));
            FLIP_STATUS(head) = 0x00000000;
        }

        // Queueing all at once
        for (int parameter = 0; parameter < 32; parameter++) {
            COMMAND[0] = METHOD(0xE940 + 4 * head, 1);
            COMMAND[1] = parameter;
            PUT += 8;

            sys_timer_usleep(1000);
            fprintf(handler, "Flip Status [DRIVER_INFO @ 0x%04X]: 0x%08X\n", 0x10C0 + 0x40 * head, FLIP_STATUS(head));
        }

        // Reset status
        FLIP_STATUS(head) = 0x00000000;

        // Queue invalid ones
        COMMAND[0] = METHOD(0xE940 + 4 * head, 1);
        COMMAND[1] = 32;
        PUT += 8;
        COMMAND[0] = METHOD(0xE940 + 4 * head, 1);
        COMMAND[1] = 0xFFFFFFFF;
        PUT += 8;

        sys_timer_usleep(1000);
        fprintf(handler, "Flip Status [DRIVER_INFO @ 0x%04X]: 0x%08X\n", 0x10C0 + 0x40 * head - 4, *(uint32_t*)(LPAR_DRIVER_INFO_ADDR + 0x10C0 + 0x40 * head - 4));
        fprintf(handler, "Flip Status [DRIVER_INFO @ 0x%04X]: 0x%08X\n", 0x10C0 + 0x40 * head + 0, *(uint32_t*)(LPAR_DRIVER_INFO_ADDR + 0x10C0 + 0x40 * head + 0));
        fprintf(handler, "Flip Status [DRIVER_INFO @ 0x%04X]: 0x%08X\n", 0x10C0 + 0x40 * head + 4, *(uint32_t*)(LPAR_DRIVER_INFO_ADDR + 0x10C0 + 0x40 * head + 4));
    }

    /**
     * TODO: Test SCE_DRIVER_FLIP (0xE920:0xE924)
     */


    fclose(handler);
    return 0;
}
