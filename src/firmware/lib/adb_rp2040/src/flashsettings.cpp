#include "flashsettings.h"
#include "pico/multicore.h"
#include "string.h"
#define STORAGE_CMD_TOTAL_BYTES 64

void FlashSettings::init(void)
{
    // Get Flash info
    uint8_t txbuf[STORAGE_CMD_TOTAL_BYTES] = {0x9f};
    uint8_t rxbuf[STORAGE_CMD_TOTAL_BYTES] = {0};
    uint32_t saved_isr_state = save_and_disable_interrupts();
    flash_do_cmd(txbuf, rxbuf, STORAGE_CMD_TOTAL_BYTES);
    restore_interrupts(saved_isr_state);
    _capacity =  1 << rxbuf[3];
    _last_sector = _capacity - FLASH_SECTOR_SIZE;

    // Read initial settings
    uint8_t* setting_buffer = read_settings_page();
    
    if (((uint16_t*)setting_buffer)[0] == QUOKKADB_SETTINGS_MAGIC_NUMBER) 
    {
        memcpy((void*)&_settings, setting_buffer, FLASH_PAGE_SIZE);
    }
    else
    {
        // set default values
        _settings.magic_number = QUOKKADB_SETTINGS_MAGIC_NUMBER;
        _settings.led_on = 1;
    }

}

void FlashSettings::write_settings_page(uint8_t *buf)
{
    multicore_lockout_start_blocking();
    uint32_t saved_isr_state = save_and_disable_interrupts();

    flash_range_erase(_last_sector, FLASH_SECTOR_SIZE);
    flash_range_program(_last_sector, buf, FLASH_PAGE_SIZE);

    restore_interrupts(saved_isr_state);
    multicore_lockout_end_blocking();
}

uint8_t* FlashSettings::read_settings_page(void)
{
    return (uint8_t*)(XIP_BASE + _last_sector);
}

void FlashSettings::save(void)
{
    write_settings_page((uint8_t*)&_settings);
}