// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_client_ll.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothubtransportmqtt.h"
#include "iothub_client_options.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "certs.h"

static const char *TAG = "IOTHUB";

#define EXAMPLE_IOTHUB_CONNECTION_STRING CONFIG_IOTHUB_CONNECTION_STRING
static const char *connectionString = EXAMPLE_IOTHUB_CONNECTION_STRING;

static void connection_status_callback(IOTHUB_CLIENT_CONNECTION_STATUS status, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void *userContextCallback)
{
    ESP_LOGI(TAG, "Connection Status Callback: status=%d reason=%d", status, reason);

    switch (reason)
    {
    case IOTHUB_CLIENT_CONNECTION_OK:
        ESP_LOGI(TAG, "IOTHUB_CLIENT_CONNECTION_OK");
        break;
    case IOTHUB_CLIENT_CONNECTION_EXPIRED_SAS_TOKEN:
        ESP_LOGW(TAG, "IOTHUB_CLIENT_CONNECTION_EXPIRED_SAS_TOKEN");
        break;
    case IOTHUB_CLIENT_CONNECTION_DEVICE_DISABLED:
        ESP_LOGW(TAG, "IOTHUB_CLIENT_CONNECTION_DEVICE_DISABLED");
        break;
    case IOTHUB_CLIENT_CONNECTION_BAD_CREDENTIAL:
        ESP_LOGW(TAG, "IOTHUB_CLIENT_CONNECTION_BAD_CREDENTIAL");
        break;
    case IOTHUB_CLIENT_CONNECTION_RETRY_EXPIRED:
        ESP_LOGW(TAG, "IOTHUB_CLIENT_CONNECTION_RETRY_EXPIRED");
        break;
    case IOTHUB_CLIENT_CONNECTION_NO_NETWORK:
        ESP_LOGW(TAG, "IOTHUB_CLIENT_CONNECTION_NO_NETWORK");
        break;
    case IOTHUB_CLIENT_CONNECTION_COMMUNICATION_ERROR:
        ESP_LOGW(TAG, "IOTHUB_CLIENT_CONNECTION_COMMUNICATION_ERROR");
        break;
    default:
        ESP_LOGW(TAG, "UNKNOWN_REASON");
    }
}

void iothub_client_sample_mqtt_run(void)
{
    IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;

    if (platform_init() != 0)
    {
        ESP_LOGE(TAG, "Failed to initialize the platform.");
    }
    else
    {
        if ((iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol)) == NULL)
        {
            ESP_LOGE(TAG, "iotHubClientHandle is NULL!");
        }
        else if (IoTHubClient_LL_SetConnectionStatusCallback(iotHubClientHandle, connection_status_callback, NULL) != IOTHUB_CLIENT_OK)
        {
            ESP_LOGE(TAG, "Failed on IoTHubClient_LL_SetConnectionStatusCallback");
        }
        else
        {
            bool traceOn = true;
            IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);
            IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates);
            // int mqttPingFreqInSecs = 15;
            // IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_KEEP_ALIVE, &mqttPingFreqInSecs);
            // int sasTokenLifetime = 60;
            // IoTHubClient_LL_SetOption(iotHubClientHandle, OPTION_SAS_TOKEN_LIFETIME, &sasTokenLifetime);

            do
            {
                IoTHubClient_LL_DoWork(iotHubClientHandle);
                ThreadAPI_Sleep(100);
            } while (true);

            IoTHubClient_LL_Destroy(iotHubClientHandle);
        }
        platform_deinit();
    }
}

int main(void)
{
    iothub_client_sample_mqtt_run();
    return 0;
}
