#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp32/rom/rtc.h"

#include "nvs_flash.h"
#include "iothub_client_sample_mqtt.h"

#define EXAMPLE_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_WIFI_PASS CONFIG_WIFI_PASSWORD

static EventGroupHandle_t wifi_event_group;

#ifndef BIT0
#define BIT0 (0x1 << 0)
#endif
const int CONNECTED_BIT = BIT0;
static const char *TAG = "MAIN";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START.");
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP.");
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_LOST_IP.");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED.");
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_CONNECTED.");
        break;
    default:
        ESP_LOGI(TAG, "UNHANDLED WIFI EVENT, EVENT_ID: %d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void initialise_wifi(void)
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void azure_task(void *pvParameter)
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to AP success!");

    iothub_client_sample_mqtt_run();

    vTaskDelete(NULL);
}

void heapPoll(void *p)
{
    time_t timer;
    char buffer[26];
    struct tm *tm_info;
    multi_heap_info_t heapInfo;
    wifi_ap_record_t wifidata;

    while (1)
    {
        time(&timer);
        tm_info = localtime(&timer);
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        esp_wifi_sta_get_ap_info(&wifidata);
        heap_caps_get_info(&heapInfo, MALLOC_CAP_8BIT);

        ESP_LOGI(TAG, "Time UTC:             %s", buffer);
        ESP_LOGI(TAG, "WIFI RSSI:            %d", wifidata.rssi);
        ESP_LOGI(TAG, "Total free bytes:     %d", heapInfo.total_free_bytes);
        ESP_LOGI(TAG, "Total allocated bytes:%d", heapInfo.total_allocated_bytes);
        ESP_LOGI(TAG, "Minimum free bytes:   %d", heapInfo.minimum_free_bytes);
        ESP_LOGI(TAG, "Largest free block :  %d", heapInfo.largest_free_block);
        ESP_LOGI(TAG, "Allocated blocks:     %d", heapInfo.allocated_blocks);
        ESP_LOGI(TAG, "Free blocks:          %d", heapInfo.free_blocks);
        ESP_LOGI(TAG, "Total blocks:         %d", heapInfo.total_blocks);
        vTaskDelay(1000 * 60 / portTICK_RATE_MS);
    }
}

void app_main()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Reset reason Core0: %d", rtc_get_reset_reason(0));
    ESP_LOGI(TAG, "Reset reason Core1: %d", rtc_get_reset_reason(1));

    initialise_wifi();

    xTaskCreatePinnedToCore(&azure_task, "azure_task", 1024 * 16, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(&heapPoll, "heapPoll", 1024 * 5, NULL, 0, NULL, 1);
}
