#include <stdio.h>
#include <sys/time.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "driver/ledc.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

#include "lvgl.h"

#include "sdkconfig.h"

#define TAG "app"

#define LVGL_TICK_PERIOD_MS 2
#define LCD_PIXEL_CLOCK_HZ (12 * 1000 * 1000)

#define LCD_H_RES 800
#define LCD_V_RES 480

/* see https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3 */
#define PIN_NUM_PCLK (gpio_num_t) GPIO_NUM_7
#define PIN_NUM_VSYNC (gpio_num_t) GPIO_NUM_3
#define PIN_NUM_HSYNC (gpio_num_t) GPIO_NUM_46
#define PIN_NUM_DE (gpio_num_t) GPIO_NUM_5

#define PIN_NUM_DATA0 (gpio_num_t) GPIO_NUM_14  /* B3 */
#define PIN_NUM_DATA1 (gpio_num_t) GPIO_NUM_38  /* B4 */
#define PIN_NUM_DATA2 (gpio_num_t) GPIO_NUM_18  /* B5 */
#define PIN_NUM_DATA3 (gpio_num_t) GPIO_NUM_17  /* B6 */
#define PIN_NUM_DATA4 (gpio_num_t) GPIO_NUM_10  /* B7 */
#define PIN_NUM_DATA5 (gpio_num_t) GPIO_NUM_39  /* G2 */
#define PIN_NUM_DATA6 (gpio_num_t) GPIO_NUM_0   /* G3 */
#define PIN_NUM_DATA7 (gpio_num_t) GPIO_NUM_45  /* G4 */
#define PIN_NUM_DATA8 (gpio_num_t) GPIO_NUM_48  /* G5 */
#define PIN_NUM_DATA9 (gpio_num_t) GPIO_NUM_47  /* G6 */
#define PIN_NUM_DATA10 (gpio_num_t) GPIO_NUM_21 /* G7 */
#define PIN_NUM_DATA11 (gpio_num_t) GPIO_NUM_1  /* R3 */
#define PIN_NUM_DATA12 (gpio_num_t) GPIO_NUM_2  /* R4 */
#define PIN_NUM_DATA13 (gpio_num_t) GPIO_NUM_42 /* R5 */
#define PIN_NUM_DATA14 (gpio_num_t) GPIO_NUM_41 /* R6 */
#define PIN_NUM_DATA15 (gpio_num_t) GPIO_NUM_40 /* R7 */

typedef struct
{
    lv_obj_t *meter;
    lv_meter_indicator_t *indicator;
} TMeter;

TMeter *lvgl_demo(lv_disp_t *disp)
{
    static TMeter lvgl_meter;

    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *meter = lv_meter_create(scr);

    lv_obj_center(meter);
    lv_obj_set_size(meter, 400, 400);

    lv_meter_scale_t *scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_range(meter, scale, 0, 60, 270, 135);
    lv_meter_set_scale_ticks(meter, scale, 5, 5, 32, lv_palette_main(LV_PALETTE_GREY));

    lv_meter_indicator_t *mi;

    mi = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(meter, mi, 0);
    lv_meter_set_indicator_end_value(meter, mi, 20);

    mi = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_meter_set_indicator_start_value(meter, mi, 20);
    lv_meter_set_indicator_end_value(meter, mi, 40);

    mi = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_ORANGE), 0);
    lv_meter_set_indicator_start_value(meter, mi, 40);
    lv_meter_set_indicator_end_value(meter, mi, 60);

    lvgl_meter.indicator = lv_meter_add_needle_line(meter, scale, 5, lv_palette_main(LV_PALETTE_PINK), 0);
    lvgl_meter.meter = meter;

    return &lvgl_meter;
}

void BacklightInit(void)
{
    ledc_timer_config_t ledTimer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_12_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK};

    ledc_channel_config_t ledChannel = {
        .gpio_num = GPIO_NUM_2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
        .flags.output_invert = 0};

    ESP_ERROR_CHECK(ledc_timer_config(&ledTimer));
    ESP_ERROR_CHECK(ledc_channel_config(&ledChannel));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 8191));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

esp_lcd_panel_handle_t DisplayInit(void)
{
    ESP_LOGI(TAG, "Install RGB LCD panel driver");

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_rgb_panel_config_t panel_config = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings = {
            .pclk_hz = LCD_PIXEL_CLOCK_HZ,
            .h_res = LCD_H_RES,
            .v_res = LCD_V_RES,
            .hsync_pulse_width = 4,
            .hsync_back_porch = 8,
            .hsync_front_porch = 8,
            .vsync_pulse_width = 4,
            .vsync_back_porch = 8,
            .vsync_front_porch = 8,
            .flags.hsync_idle_low = true,
            .flags.vsync_idle_low = true,
            .flags.de_idle_high = false,
            .flags.pclk_active_neg = true,
            .flags.pclk_idle_high = false},
        .data_width = 16,
        .sram_trans_align = 8,
        .psram_trans_align = 64,
        .hsync_gpio_num = PIN_NUM_HSYNC,
        .vsync_gpio_num = PIN_NUM_VSYNC,
        .de_gpio_num = PIN_NUM_DE,
        .pclk_gpio_num = PIN_NUM_PCLK,
        .data_gpio_nums = {
            PIN_NUM_DATA0,
            PIN_NUM_DATA1,
            PIN_NUM_DATA2,
            PIN_NUM_DATA3,
            PIN_NUM_DATA4,
            PIN_NUM_DATA5,
            PIN_NUM_DATA6,
            PIN_NUM_DATA7,
            PIN_NUM_DATA8,
            PIN_NUM_DATA9,
            PIN_NUM_DATA10,
            PIN_NUM_DATA11,
            PIN_NUM_DATA12,
            PIN_NUM_DATA13,
            PIN_NUM_DATA14,
            PIN_NUM_DATA15,
        },
        .disp_gpio_num = GPIO_NUM_NC,
        .flags.disp_active_low = 0,
        .flags.fb_in_psram = true};
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    return panel_handle;
}

static void lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;

    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
    lv_disp_flush_ready(drv);
}

static void increase_lvgl_tick(void *arg)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

void app_main(void)
{
    static esp_lcd_panel_handle_t panel_handle;
    TMeter *lvgl_meter;

    ESP_LOGI(TAG, "Initialize hardware");
    BacklightInit();
    panel_handle = DisplayInit();

    ESP_LOGI(TAG, "Initialize LVGL library");
    static lv_disp_draw_buf_t disp_buf;
    static lv_disp_drv_t disp_drv;
    lv_init();

    ESP_LOGI(TAG, "Allocate separate LVGL draw buffers from PSRAM");
    void *buf1 = NULL;
    void *buf2 = NULL;

    buf1 = heap_caps_malloc(LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf1);
    buf2 = heap_caps_malloc(LCD_H_RES * 100 * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    assert(buf2);

    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_H_RES * 100);

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_H_RES;
    disp_drv.ver_res = LCD_V_RES;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    disp_drv.full_refresh = false;

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "Install LVGL tick timer");
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lvgl_tick",
        .skip_unhandled_events = true};

    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

    ESP_LOGI(TAG, "LVGL demo");
    lvgl_meter = lvgl_demo(disp);

    uint8_t lastSec = 0;
    struct timeval tv_now;

    while (42)
    {
        gettimeofday(&tv_now, NULL);

        if (lastSec != tv_now.tv_sec % 60)
        {
            lastSec = tv_now.tv_sec % 60;
            lv_meter_set_indicator_value(lvgl_meter->meter, lvgl_meter->indicator, lastSec);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
        lv_timer_handler();
    }
}
