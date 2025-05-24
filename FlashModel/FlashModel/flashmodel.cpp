#include "flashmodel.h"
#include <stdint.h>
#include <stdexcept>
#include <string>

// allocate memory for flash
static uint8_t FLASH_MODEL[PICO_FLASH_SIZE_BYTES];

void FlashModelInit()
{
	memset(FLASH_MODEL, 0xFF, sizeof(FLASH_MODEL));
}

void *FlashBasePtr()
{
	return FLASH_MODEL;
}

void flash_range_program(uint32_t flash_offs, const uint8_t* data, size_t count)
{
	if (flash_offs & (FLASH_PAGE_SIZE - 1)) {
		throw std::invalid_argument("Flash Offset must be a multiple of page. got " + std::to_string(flash_offs));
	}
	if (count & (FLASH_PAGE_SIZE - 1)) {
		throw std::invalid_argument("Byte count must be a multiple of page. got " + std::to_string(count));
	}
	if (flash_offs + count > sizeof(FLASH_MODEL)) {
		throw std::invalid_argument("Out of bounds for flash: offset " + std::to_string(flash_offs) + ", size " + std::to_string(count));
	}
	
	memcpy(FLASH_MODEL + flash_offs, data, count);
}

void flash_range_erase(uint32_t flash_offs, size_t count)
{
	if (flash_offs & (FLASH_PAGE_SIZE - 1)) {
		throw std::invalid_argument("Flash offset must be a multiple of sector. got " + std::to_string(flash_offs));
	}
	if (count & (FLASH_PAGE_SIZE - 1)) {
		throw std::invalid_argument("Byte count must be a multiple of sector. got " + std::to_string(count));
	}
	if (flash_offs + count > sizeof(FLASH_MODEL)) {
		throw std::invalid_argument("Out of bounds for flash: offset " + std::to_string(flash_offs) + ", size " + std::to_string(count));
	}

	memset(FLASH_MODEL + flash_offs, 0xFF, count);
}

