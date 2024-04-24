#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_idf_version.h>
#include <max7219.h>

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(4, 0, 0)
#define HOST HSPI_HOST
#else
#define HOST SPI2_HOST
#endif

#define CONFIG_CASCADE_SIZE 1
#define CONFIG_PIN_NUM_MOSI 23
#define CONFIG_PIN_NUM_CLK 18
#define CONFIG_PIN_CS 21
#define CONFIG_DELAY 500

static const uint64_t led1 = 0x001;
static const uint64_t led2 = 0x010;
static const uint64_t led3 = 0x011;
static const uint64_t led4 = 0x100;

void task(void *pvParameter)
{
    // Configure SPI bus
    spi_bus_config_t cfg = {
        .mosi_io_num = CONFIG_PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = CONFIG_PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
        .flags = 0};
    ESP_ERROR_CHECK(spi_bus_initialize(HOST, &cfg, 1));

    // Configure device
    max7219_t dev = {
        .cascade_size = CONFIG_CASCADE_SIZE,
        .digits = 8,
        .mirrored = false};
    ESP_ERROR_CHECK(max7219_init_desc(&dev, HOST, MAX7219_MAX_CLOCK_SPEED_HZ, CONFIG_PIN_CS));
    ESP_ERROR_CHECK(max7219_init(&dev));

    for (;;)
    {
        printf("LED: 1\n");
        max7219_draw_image_8x8(&dev, 1, (void*)(&led1));
        vTaskDelay(pdMS_TO_TICKS(CONFIG_DELAY));

        printf("LED: 2\n");
        max7219_draw_image_8x8(&dev, 1, (void*)(&led2));
        vTaskDelay(pdMS_TO_TICKS(CONFIG_DELAY));

        printf("LED: 3\n");
        max7219_draw_image_8x8(&dev, 1, (void*)(&led3));
        vTaskDelay(pdMS_TO_TICKS(CONFIG_DELAY));

        printf("LED: 4\n");
        max7219_draw_image_8x8(&dev, 1, (void*)(&led4));
        vTaskDelay(pdMS_TO_TICKS(CONFIG_DELAY));
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(task, "task", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL, APP_CPU_NUM);
}
