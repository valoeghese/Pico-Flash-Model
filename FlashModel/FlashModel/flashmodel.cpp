#include "flashmodel.h"
#include <stdint.h>
#include <stdexcept>
#include <string>
#include <stdio.h>

// allocate memory for flash
static uint8_t FLASH_MODEL[PICO_FLASH_SIZE_BYTES];
static bool interrupts_enabled = 1;

void FlashModelInit()
{
	memset(FLASH_MODEL, 0xFF, sizeof(FLASH_MODEL));
}

void FlashModelDumpPage(uint32_t flash_offs)
{
	if (flash_offs & (FLASH_PAGE_SIZE - 1)) {
		throw std::invalid_argument("Flash Offset must be a multiple of page. got " + std::to_string(flash_offs));
	}
	if ((uintptr_t)flash_offs + FLASH_PAGE_SIZE > sizeof(FLASH_MODEL)) {
		throw std::invalid_argument("Out of bounds for flash: offset " + std::to_string(flash_offs));
	}

	for (size_t i = 0; i < FLASH_PAGE_SIZE; i++) {
		printf("%02x ", FLASH_MODEL[flash_offs + i]);
		if ((i & 15) == 15)
			printf("\n");
	}
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
#ifdef REQUIRE_DISABLED_INTERRUPT
	if (interrupts_enabled) {
		throw std::runtime_error("Interrupts are enabled while programming flash!");
	}
#endif

#if FAST_FLASH
	memcpy(FLASH_MODEL + flash_offs, data, count);
#else
	for (size_t i = 0; i < count; i++) {
		FLASH_MODEL[flash_offs + count] &= data[i];
	}
#endif
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
#if REQUIRE_DISABLED_INTERRUPT
	if (interrupts_enabled) {
		throw std::runtime_error("Interrupts are enabled while erasing flash!");
	}
#endif

	memset(FLASH_MODEL + flash_offs, 0xFF, count);
}

uint32_t save_and_disable_interrupts()
{
	interrupts_enabled = 0;
	return 1;
}
void restore_interrupts(uint32_t interrupts)
{
	interrupts_enabled = 1;
}
