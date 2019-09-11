// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "azure_c_shared_utility/platform.h"
#include "sntp.h"
#include "azure_c_shared_utility/tlsio_mbedtls.h"
#include "azure_c_shared_utility/xlogging.h"

static const char *const ntpServer = "pool.ntp.org";

int platform_init(void)
{
    if (SNTP_SetServerName(ntpServer) != 0)
    {
        LogError("Failed SNTP_SetServerName");
        return MU_FAILURE;
    }
    if (SNTP_Init() != 0)
    {
        LogError("Failed SNTP_Init");
        return MU_FAILURE;
    }

    return 0;
}

const IO_INTERFACE_DESCRIPTION *platform_get_default_tlsio(void)
{
    return tlsio_mbedtls_get_interface_description();
}

STRING_HANDLE platform_get_platform_info(PLATFORM_INFO_OPTION options)
{
    (void)options;

    return STRING_construct("(esp_idf_mbedtls_berkeley)");
}

void platform_deinit(void)
{
    SNTP_Deinit();
}