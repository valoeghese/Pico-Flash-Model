#pragma once

#include <stdint.h>
#include <string.h>

// This should work for modelling Pico and all pico-derived microcontroller's flash.
// Should also be easily adaptable to microcontrollers with a similar api.

// When this is enabled, an exception will be thrown if pico interrupts are not disabled before flash program or erase.
#define REQUIRE_DISABLED_INTERRUPT 1
// use memset instead of looping byte-by-byte with bitwise & for slight speed improvement. less accurate to real flash program behaviour
#define FAST_FLASH 0

// parameters, Change these how you want for testing by setting defines.
// I recommend reducing from the actual sizes on the Pico (W) if you want to use less / more constrained memory for testing.
#ifndef FLASH_PAGE_SIZE
	#define FLASH_PAGE_SIZE (1u << 8)        // 256u
#endif
#ifndef FLASH_SECTOR_SIZE
	#define FLASH_SECTOR_SIZE (1u << 12)      // 4096u
#endif
#ifndef PICO_FLASH_SIZE_BYTES
	#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024) // 2 MiB
#endif

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