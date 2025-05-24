#pragma once

#include <stdint.h>
#include <string.h>

// This should work for modelling Pico and all pico-derived microcontroller's flash.
// Should also be easily adaptable to microcontrollers with a similar api.

// When this is enabled, an exception will be thrown if pico interrupts are not disabled before flash program or erase.
#define REQUIRE_DISABLED_INTERRUPT 1
// use memset instead of looping byte-by-byte and bitwise & for slight speed improvement
#define FAST_FLASH 0

// parameters, Change these how you want for testing. These are currently reduced from the actual sizes on the Pico (W).
// Pico W has page: (1u << 8) == 256u, sector: (1u << 12) == 4096u. We have less for testing.
#define FLASH_PAGE_SIZE (1u << 6)        // 64u
#define FLASH_SECTOR_SIZE (1u << 8)      // 256u

// Pico W has 2MiB. We have smaller for testing (especially because it is statically allocated).
#define PICO_FLASH_SIZE_BYTES (1 * 1024) // 1 KiB

#define XIP_BASE ((uintptr_t)FlashBasePtr())

// Initialise the flash model (set everything to 1)
void FlashModelInit();

// Get the base pointer to flash.
void *FlashBasePtr();

// Debug: print all bytes of a given page to standard output.
void FlashModelDumpPage(uint32_t flash_offs);

// Define the Pico W's interface for flash.

// @param flash_offs The offset into flash to write. This must be a multiple of one page.
// @param data Pointer to the data to write.
// @param count The number of bytes to write. This must be a multiple of one page.
void flash_range_program(uint32_t flash_offs, const uint8_t* data, size_t count);

// @param flash_offs The offset into flash to erase. This must be a multiple of one sector.
// @param count The number of bytes to erase. This must be a multiple of one sector.
void flash_range_erase(uint32_t flash_offs, size_t count);

// Pico interface for saving and restoring interrupts. This implementation for testing does not modify any interrupts on the device.
// Recommended to use when performing flash operations. 
// https://kevinboone.me/picoflash.html
uint32_t save_and_disable_interrupts();

void restore_interrupts(uint32_t interrupts);