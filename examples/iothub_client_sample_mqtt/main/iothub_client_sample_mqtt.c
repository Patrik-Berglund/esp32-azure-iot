// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "iothub_device_client_ll.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/shared_util_options.h"
#include "iothub_client_options.h"
#include "certs.h"
#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"

#define SAMPLE_MQTT
// #define SAMPLE_MQTT_OVER_WEBSOCKETS
// #define SAMPLE_AMQP
// #define SAMPLE_AMQP_OVER_WEBSOCKETS
// #define SAMPLE_HTTP

#ifdef SAMPLE_MQTT
#include "iothubtransportmqtt.h"
#include "azure_prov_client/prov_transport_mqtt_client.h"
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
#include "iothubtransportmqtt_websockets.h"
#include "azure_prov_client/prov_transport_mqtt_ws_client.h"
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
#include "iothubtransportamqp.h"
#include "azure_prov_client/prov_transport_amqp_client.h"
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
#include "iothubtransportamqp_websockets.h"
#include "azure_prov_client/prov_transport_amqp_ws_client.h"
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
#include "iothubtransporthttp.h"
#include "azure_prov_client/prov_transport_http_client.h"
#endif // SAMPLE_HTTP

typedef struct CLIENT_SAMPLE_INFO_TAG
{
    char *iothub_uri;
    char *device_id;
    int registration_complete;
} CLIENT_SAMPLE_INFO;

#define EXAMPLE_IOTHUB_DPS_PROVISIONING_URI CONFIG_IOTHUB_DPS_PROVISIONING_URI
#define EXAMPLE_IOTHUB_DPS_ID_SCOPE CONFIG_IOTHUB_DPS_ID_SCOPE
#define EXAMPLE_IOTHUB_DPS_REGISTRATION_NAME CONFIG_IOTHUB_DPS_REGISTRATION_NAME
#define EXAMPLE_IOTHUB_DPS_PRIMARY_KEY CONFIG_IOTHUB_DPS_PRIMARY_KEY
#define EXAMPLE_IOTHUB_DPS_SECONDARY_KEY CONFIG_IOTHUB_DPS_SECONDARY_KEY

static const char *TAG = "IOTHUB";
static CLIENT_SAMPLE_INFO user_ctx;

MU_DEFINE_ENUM_STRINGS(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_VALUE);
MU_DEFINE_ENUM_STRINGS(PROV_DEVICE_REG_STATUS, PROV_DEVICE_REG_STATUS_VALUES);

static void connection_status_callback(IOTHUB_CLIENT_CONNECTION_STATUS status, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void *userContextCallback)
{
    ESP_LOGI(TAG, "Connection Status Callback, Status: %s Reason: %s", MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONNECTION_STATUS, status), MU_ENUM_TO_STRING(IOTHUB_CLIENT_CONNECTION_STATUS_REASON, reason));
}

static void register_device_callback(PROV_DEVICE_RESULT register_result, const char *iothub_uri, const char *device_id, void *user_context)
{
    ESP_LOGI(TAG, "Registration result: %s", MU_ENUM_TO_STRING(PROV_DEVICE_RESULT, register_result));

    CLIENT_SAMPLE_INFO *user_ctx = (CLIENT_SAMPLE_INFO *)user_context;
    if (register_result == PROV_DEVICE_RESULT_OK)
    {
        ESP_LOGI(TAG, "Registration Information received from service: %s, deviceId: %s", iothub_uri, device_id);
        (void)mallocAndStrcpy_s(&user_ctx->iothub_uri, iothub_uri);
        (void)mallocAndStrcpy_s(&user_ctx->device_id, device_id);

        user_ctx->registration_complete = 1;
    }
    else
    {
        ESP_LOGW(TAG, "Failure encountered on registration %s", MU_ENUM_TO_STRING(PROV_DEVICE_RESULT, register_result));
        user_ctx->registration_complete = 2;
    }
}

static void registration_status_callback(PROV_DEVICE_REG_STATUS reg_status, void *user_context)
{
    ESP_LOGI(TAG, "Provisioning Status: %s", MU_ENUM_TO_STRING(PROV_DEVICE_REG_STATUS, reg_status));
}

static void bootstrap_device(void)
{
    PROV_DEVICE_LL_HANDLE handle;
    PROV_DEVICE_TRANSPORT_PROVIDER_FUNCTION prov_transport;
    bool usePrimaryKey = true;
    uint8_t timeout = 60;

#ifdef SAMPLE_MQTT
    prov_transport = Prov_Device_MQTT_Protocol;
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
    prov_transport = Prov_Device_MQTT_WS_Protocol;
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
    prov_transport = Prov_Device_AMQP_Protocol;
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
    prov_transport = Prov_Device_AMQP_WS_Protocol;
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
    prov_transport = Prov_Device_HTTP_Protocol;
#endif // SAMPLE_HTTP

    do
    {
        memset(&user_ctx, 0, sizeof(CLIENT_SAMPLE_INFO));

        prov_dev_security_init(SECURE_DEVICE_TYPE_SYMMETRIC_KEY);
        prov_dev_set_symmetric_key_info(EXAMPLE_IOTHUB_DPS_REGISTRATION_NAME, (usePrimaryKey) ? EXAMPLE_IOTHUB_DPS_PRIMARY_KEY : EXAMPLE_IOTHUB_DPS_SECONDARY_KEY);

        if ((handle = Prov_Device_LL_Create(EXAMPLE_IOTHUB_DPS_PROVISIONING_URI, EXAMPLE_IOTHUB_DPS_ID_SCOPE, prov_transport)) == NULL)
        {
            ESP_LOGE(TAG, "failed calling Prov_Device_LL_Create");
        }
        else
        {
#ifndef SAMPLE_HTTP
            bool traceOn = true;
            Prov_Device_LL_SetOption(handle, PROV_OPTION_LOG_TRACE, &traceOn);
#endif
            Prov_Device_LL_SetOption(handle, OPTION_TRUSTED_CERT, certificates);
            Prov_Device_LL_SetOption(handle, PROV_OPTION_TIMEOUT, &timeout);

            if (Prov_Device_LL_Register_Device(handle, register_device_callback, &user_ctx, registration_status_callback, &user_ctx) != PROV_DEVICE_RESULT_OK)
            {
                ESP_LOGE(TAG, "failed calling Prov_Device_LL_Register_Device");
            }
            else
            {
                do
                {
                    Prov_Device_LL_DoWork(handle);
                    ThreadAPI_Sleep(10);
                } while (user_ctx.registration_complete == 0);
            }
            Prov_Device_LL_Destroy(handle);
        }

        if (user_ctx.registration_complete != 1)
        {
            ThreadAPI_Sleep(30 * 1000);
            ESP_LOGI(TAG, "Swapping key");
            usePrimaryKey = !usePrimaryKey;
            prov_dev_security_deinit();

            free(user_ctx.device_id);
            free(user_ctx.iothub_uri);
        }
    } while (user_ctx.registration_complete != 1);
}

void iothub_client_sample_mqtt_run(void)
{
    IOTHUB_DEVICE_CLIENT_LL_HANDLE iotHubClientHandle;
    IOTHUB_CLIENT_TRANSPORT_PROVIDER iothub_transport;

#ifdef SAMPLE_MQTT
    iothub_transport = MQTT_Protocol;
#endif // SAMPLE_MQTT
#ifdef SAMPLE_MQTT_OVER_WEBSOCKETS
    iothub_transport = MQTT_WebSocket_Protocol;
#endif // SAMPLE_MQTT_OVER_WEBSOCKETS
#ifdef SAMPLE_AMQP
    iothub_transport = AMQP_Protocol;
#endif // SAMPLE_AMQP
#ifdef SAMPLE_AMQP_OVER_WEBSOCKETS
    iothub_transport = AMQP_Protocol_over_WebSocketsTls;
#endif // SAMPLE_AMQP_OVER_WEBSOCKETS
#ifdef SAMPLE_HTTP
    iothub_transport = HTTP_Protocol;
#endif // SAMPLE_HTTP

    if (platform_init() != 0)
    {
        ESP_LOGE(TAG, "Failed to initialize the platform.");
    }
    else
    {
        bootstrap_device();

        if ((iotHubClientHandle = IoTHubDeviceClient_LL_CreateFromDeviceAuth(user_ctx.iothub_uri, user_ctx.device_id, iothub_transport)) == NULL)
        {
            ESP_LOGE(TAG, "iotHubClientHandle is NULL!");
        }
        else if (IoTHubDeviceClient_LL_SetConnectionStatusCallback(iotHubClientHandle, connection_status_callback, NULL) != IOTHUB_CLIENT_OK)
        {
            ESP_LOGE(TAG, "Failed on IoTHubDeviceClient_LL_SetConnectionStatusCallback");
        }
        else
        {
#ifndef SAMPLE_HTTP
            bool traceOn = true;
            IoTHubDeviceClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);
#endif
            IoTHubDeviceClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates);

            ESP_LOGI(TAG, "IoTHubDeviceClient_LL_DoWork started");
            do
            {
                IoTHubDeviceClient_LL_DoWork(iotHubClientHandle);
                ThreadAPI_Sleep(10);
            } while (true);

            IoTHubDeviceClient_LL_Destroy(iotHubClientHandle);
        }
        prov_dev_security_deinit();
        platform_deinit();
    }
}

int main(void)
{
    iothub_client_sample_mqtt_run();
    return 0;
}
