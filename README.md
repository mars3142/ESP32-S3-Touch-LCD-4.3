# Waveshare ESP32-S3-Touch-LCD-4.3

## Information

- Orderlink: https://www.waveshare.com/esp32-s3-touch-lcd-4.3.htm
- Wiki: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3

## Monitor output

```
--- esp-idf-monitor 1.4.0 on /dev/cu.usbport 115200 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
ESP-ROM:esp32s3-20210327
Build:Mar 27 2021
rst:0x15 (USB_UART_CHIP_RESET),boot:0x8 (SPI_FAST_FLASH_BOOT)
Saved PC:0x40378aca
0x40378aca: esp_cpu_wait_for_intr at <esp-idf-path>/components/esp_hw_support/cpu.c:145

SPIWP:0xee
mode:DIO, clock div:1
load:0x3fce3810,len:0x178c
load:0x403c9700,len:0x4
load:0x403c9704,len:0xcbc
load:0x403cc700,len:0x2d9c
entry 0x403c9914
I (26) boot: ESP-IDF v5.2.1-343-gf9163c6d43 2nd stage bootloader
I (27) boot: compile time Apr 19 2024 15:22:33
I (27) boot: Multicore bootloader
I (31) boot: chip revision: v0.2
I (35) boot.esp32s3: Boot SPI Speed : 80MHz
I (40) boot.esp32s3: SPI Mode       : DIO
I (44) boot.esp32s3: SPI Flash Size : 8MB
I (49) boot: Enabling RNG early entropy source...
I (55) boot: Partition Table:
I (58) boot: ## Label            Usage          Type ST Offset   Length
I (65) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (73) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (80) boot:  2 factory          factory app      00 00 00010000 00100000
I (88) boot: End of partition table
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c050020 size=134a4h ( 79012) map
I (115) esp_image: segment 1: paddr=000234cc vaddr=3fc94500 size=03268h ( 12904) load
I (118) esp_image: segment 2: paddr=0002673c vaddr=40374000 size=098dch ( 39132) load
I (129) esp_image: segment 3: paddr=00030020 vaddr=42000020 size=46ba4h (289700) map
I (181) esp_image: segment 4: paddr=00076bcc vaddr=4037d8dc size=06c10h ( 27664) load
I (195) boot: Loaded app from partition at offset 0x10000
I (195) boot: Disabling RNG early entropy source...
I (206) cpu_start: Multicore app
I (206) octal_psram: vendor id    : 0x0d (AP)
I (206) octal_psram: dev id       : 0x02 (generation 3)
I (209) octal_psram: density      : 0x03 (64 Mbit)
I (215) octal_psram: good-die     : 0x01 (Pass)
I (220) octal_psram: Latency      : 0x01 (Fixed)
I (225) octal_psram: VCC          : 0x01 (3V)
I (230) octal_psram: SRF          : 0x01 (Fast Refresh)
I (236) octal_psram: BurstType    : 0x01 (Hybrid Wrap)
I (242) octal_psram: BurstLen     : 0x01 (32 Byte)
I (247) octal_psram: Readlatency  : 0x02 (10 cycles@Fixed)
I (254) octal_psram: DriveStrength: 0x00 (1/1)
I (259) MSPI Timing: PSRAM timing tuning index: 5
I (264) esp_psram: Found 8MB PSRAM device
I (269) esp_psram: Speed: 80MHz
I (306) mmu_psram: Instructions copied and mapped to SPIRAM
I (320) mmu_psram: Read only data copied and mapped to SPIRAM
I (748) esp_psram: SPI SRAM memory test OK
I (757) cpu_start: Pro cpu start user code
I (757) cpu_start: cpu freq: 160000000 Hz
I (757) cpu_start: Application information:
I (760) cpu_start: Project name:     ESP32-S3-LCD-4.3
I (765) cpu_start: App version:      1
I (770) cpu_start: Compile time:     Apr 19 2024 15:22:19
I (776) cpu_start: ELF file SHA256:  2a824ba9f...
I (781) cpu_start: ESP-IDF:          v5.2.1-343-gf9163c6d43
I (788) cpu_start: Min chip rev:     v0.0
I (792) cpu_start: Max chip rev:     v0.99
I (797) cpu_start: Chip rev:         v0.2
I (802) heap_init: Initializing. RAM available for dynamic allocation:
I (809) heap_init: At 3FCA0610 len 00049100 (292 KiB): RAM
I (815) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (821) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (827) heap_init: At 600FE010 len 00001FD8 (7 KiB): RTCRAM
I (834) esp_psram: Adding pool of 7744K of PSRAM memory to heap allocator
I (842) spi_flash: detected chip: gd
I (845) spi_flash: flash io: dio
I (850) sleep: Configure to isolate all GPIO pins in sleep state
I (856) sleep: Enable automatic switching of GPIO sleep configuration
I (864) main_task: Started on CPU0
I (874) esp_psram: Reserving pool of 32K of internal memory for DMA/internal allocations
I (874) main_task: Calling app_main()
I (884) app: Initialize hardware
I (884) app: Install RGB LCD panel driver
I (914) app: Initialize LVGL library
I (914) app: Allocate separate LVGL draw buffers from PSRAM
I (914) app: Register display driver to LVGL
I (924) app: Install LVGL tick timer
I (924) app: LVGL demo
```
