#pragma once

// parameters, change these if you want
// Pico W has page: (1u << 8) == 256u, sector: (1u << 12) == 4096u. We have less for testing.
#define FLASH_PAGE_SIZE (1u << 6)        // 64u
#define FLASH_SECTOR_SIZE (1u << 8)      // 256u

// Pico W has 2MiB. We have smaller for testing (especially because it is statically allocated).
#define PICO_FLASH_SIZE_BYTES (1 * 1024) // 1 KiB

