// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "iothub_device_client_ll.h"
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

#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/prov_transport_mqtt_client.h"

static const char *TAG = "IOTHUB";

typedef struct CLIENT_SAMPLE_INFO_TAG
{
    unsigned int sleep_time;
    char *iothub_uri;
    char *access_key_name;
    char *device_key;
    char *device_id;
    int registration_complete;
} CLIENT_SAMPLE_INFO;

static const char *global_prov_uri = "";
static const char *id_scope = "";
static const char *registration_name = "";
static const char *symmetric_key = "";

MU_DEFINE_ENUM_STRINGS(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_VALUE);
MU_DEFINE_ENUM_STRINGS(PROV_DEVICE_REG_STATUS, PROV_DEVICE_REG_STATUS_VALUES);

CLIENT_SAMPLE_INFO user_ctx;

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

static void register_device_callback(PROV_DEVICE_RESULT register_result, const char *iothub_uri, const char *device_id, void *user_context)
{
    (void)printf("Registration result: %s\r\n", MU_ENUM_TO_STRING(PROV_DEVICE_RESULT, register_result));

    CLIENT_SAMPLE_INFO *user_ctx = (CLIENT_SAMPLE_INFO *)user_context;
    if (register_result == PROV_DEVICE_RESULT_OK)
    {
        (void)printf("\r\nRegistration Information received from service: %s, deviceId: %s\r\n", iothub_uri, device_id);
        (void)mallocAndStrcpy_s(&user_ctx->iothub_uri, iothub_uri);
        (void)mallocAndStrcpy_s(&user_ctx->device_id, device_id);
        user_ctx->registration_complete = 1;
    }
    else
    {
        (void)printf("Failure encountered on registration %s\r\n", MU_ENUM_TO_STRING(PROV_DEVICE_RESULT, register_result));
        user_ctx->registration_complete = 2;
    }
}

static void registration_status_callback(PROV_DEVICE_REG_STATUS reg_status, void *user_context)
{
    (void)user_context;
    (void)printf("Provisioning Status: %s\r\n", MU_ENUM_TO_STRING(PROV_DEVICE_REG_STATUS, reg_status));
}

int bootstrap_device(void)
{
    int success = 0;
    bool traceOn = true;
    SECURE_DEVICE_TYPE hsm_type;
    hsm_type = SECURE_DEVICE_TYPE_SYMMETRIC_KEY;

    (void)prov_dev_security_init(hsm_type);
    prov_dev_set_symmetric_key_info(registration_name, symmetric_key);

    memset(&user_ctx, 0, sizeof(CLIENT_SAMPLE_INFO));

    PROV_DEVICE_LL_HANDLE handle;
    if ((handle = Prov_Device_LL_Create(global_prov_uri, id_scope, Prov_Device_MQTT_Protocol)) == NULL)
    {
        (void)printf("failed calling Prov_Device_LL_Create\r\n");
    }
    else
    {
        Prov_Device_LL_SetOption(handle, PROV_OPTION_LOG_TRACE, &traceOn);
        Prov_Device_LL_SetOption(handle, OPTION_TRUSTED_CERT, certificates);

        // This option sets the registration ID it overrides the registration ID that is
        // set within the HSM so be cautious if setting this value
        //Prov_Device_SetOption(prov_device_handle, PROV_REGISTRATION_ID, "[REGISTRATION ID]");

        if (Prov_Device_LL_Register_Device(handle, register_device_callback, &user_ctx, registration_status_callback, &user_ctx) != PROV_DEVICE_RESULT_OK)
        {
            (void)printf("failed calling Prov_Device_LL_Register_Device\r\n");
        }
        else
        {
            do
            {
                Prov_Device_LL_DoWork(handle);
                ThreadAPI_Sleep(10);
            } while (user_ctx.registration_complete == 0);

            success = 1;
        }
        Prov_Device_LL_Destroy(handle);
    }

    prov_dev_security_deinit();

    return success;
}

void iothub_client_sample_mqtt_run(void)
{
    IOTHUB_DEVICE_CLIENT_LL_HANDLE iotHubClientHandle;

    if (platform_init() != 0)
    {
        ESP_LOGE(TAG, "Failed to initialize the platform.");
    }
    else if (bootstrap_device())
    {
        if ((iotHubClientHandle = IoTHubDeviceClient_LL_CreateFromDeviceAuth(user_ctx.iothub_uri, user_ctx.device_id, MQTT_Protocol)) == NULL)
        {
            ESP_LOGE(TAG, "iotHubClientHandle is NULL!");
        }
        else if (IoTHubDeviceClient_LL_SetConnectionStatusCallback(iotHubClientHandle, connection_status_callback, NULL) != IOTHUB_CLIENT_OK)
        {
            ESP_LOGE(TAG, "Failed on IoTHubDeviceClient_LL_SetConnectionStatusCallback");
        }
        else
        {
            bool traceOn = true;
            IoTHubDeviceClient_LL_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);
            IoTHubDeviceClient_LL_SetOption(iotHubClientHandle, OPTION_TRUSTED_CERT, certificates);

            do
            {
                IoTHubDeviceClient_LL_DoWork(iotHubClientHandle);
                ThreadAPI_Sleep(10);
            } while (true);

            IoTHubDeviceClient_LL_Destroy(iotHubClientHandle);
        }
        platform_deinit();
    }
}

int main(void)
{
    iothub_client_sample_mqtt_run();
    return 0;
}
