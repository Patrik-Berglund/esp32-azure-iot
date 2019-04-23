#
# Component Makefile
#
 
# Component configuration in preprocessor defines
CFLAGS += -DUSE_LWIP_SOCKET_FOR_AZURE_IOT -Duse_amqp=OFF -Duse_http=OFF -DHSM_TYPE_SYMM_KEY
CFLAGS += -Wno-unused-function -Wno-missing-braces -Wno-missing-field-initializers

COMPONENT_ADD_INCLUDEDIRS := \
port \
port/inc \
azure-iot-sdk-c/certs \
azure-iot-sdk-c/c-utility/inc \
azure-iot-sdk-c/c-utility/inc/azure_c_shared_utility \
azure-iot-sdk-c/c-utility/pal/inc \
azure-iot-sdk-c/c-utility/pal/freertos \
azure-iot-sdk-c/c-utility/pal/generic \
azure-iot-sdk-c/iothub_client/inc \
azure-iot-sdk-c/umqtt/inc \
azure-iot-sdk-c/umqtt/inc/azure_umqtt_c \
azure-iot-sdk-c/deps/parson \
azure-iot-sdk-c/provisioning_client/inc \
azure-iot-sdk-c/provisioning_client/azure_prov_client \
azure-iot-sdk-c/provisioning_client/adapters \
 
COMPONENT_OBJS = \
azure-iot-sdk-c/certs/certs.o \
\
\
azure-iot-sdk-c/c-utility/pal/freertos/lock.o \
azure-iot-sdk-c/c-utility/pal/dns_async.o \
azure-iot-sdk-c/c-utility/pal/socket_async.o \
azure-iot-sdk-c/c-utility/pal/freertos/threadapi.o \
azure-iot-sdk-c/c-utility/pal/freertos/tickcounter.o \
azure-iot-sdk-c/c-utility/pal/lwip/sntp_lwip.o	\
\
\
azure-iot-sdk-c/c-utility/src/http_proxy_io.o \
azure-iot-sdk-c/c-utility/src/base32.o \
azure-iot-sdk-c/c-utility/adapters/agenttime.o	\
\
\
port/src/platform_openssl_compact.o \
port/src/mbed_wait_api.o \
azure-iot-sdk-c/c-utility/adapters/tlsio_mbedtls.o	\
port/src/socketio_berkeley.o	\
\
\
azure-iot-sdk-c/c-utility/src/xlogging.o \
azure-iot-sdk-c/c-utility/src/singlylinkedlist.o \
azure-iot-sdk-c/c-utility/src/buffer.o \
azure-iot-sdk-c/c-utility/src/consolelogger.o \
azure-iot-sdk-c/c-utility/src/constbuffer.o \
azure-iot-sdk-c/c-utility/src/constmap.o \
azure-iot-sdk-c/c-utility/src/crt_abstractions.o \
azure-iot-sdk-c/c-utility/src/doublylinkedlist.o \
azure-iot-sdk-c/c-utility/src/gballoc.o \
azure-iot-sdk-c/c-utility/src/gb_stdio.o \
azure-iot-sdk-c/c-utility/src/gb_time.o \
azure-iot-sdk-c/c-utility/src/hmac.o \
azure-iot-sdk-c/c-utility/src/hmacsha256.o \
azure-iot-sdk-c/c-utility/src/httpapiex.o \
azure-iot-sdk-c/c-utility/src/httpapiexsas.o \
azure-iot-sdk-c/c-utility/src/httpheaders.o \
azure-iot-sdk-c/c-utility/src/map.o \
azure-iot-sdk-c/c-utility/src/optionhandler.o \
azure-iot-sdk-c/c-utility/src/sastoken.o \
azure-iot-sdk-c/c-utility/src/sha1.o \
azure-iot-sdk-c/c-utility/src/sha224.o \
azure-iot-sdk-c/c-utility/src/sha384-512.o \
azure-iot-sdk-c/c-utility/src/strings.o \
azure-iot-sdk-c/c-utility/src/string_tokenizer.o \
azure-iot-sdk-c/c-utility/src/urlencode.o \
azure-iot-sdk-c/c-utility/src/usha.o \
azure-iot-sdk-c/c-utility/src/vector.o \
azure-iot-sdk-c/c-utility/src/xio.o \
azure-iot-sdk-c/c-utility/src/azure_base64.o \
\
\
azure-iot-sdk-c/iothub_client/src/iothub_device_client_ll.o \
azure-iot-sdk-c/iothub_client/src/iothub_client_ll.o \
azure-iot-sdk-c/iothub_client/src/iothub_client_core_ll.o \
azure-iot-sdk-c/iothub_client/src/iothub_client_ll_uploadtoblob.o \
azure-iot-sdk-c/iothub_client/src/iothub_client_authorization.o \
azure-iot-sdk-c/iothub_client/src/iothub_client_retry_control.o \
azure-iot-sdk-c/iothub_client/src/iothub_client_diagnostic.o \
azure-iot-sdk-c/iothub_client/src/iothub_message.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportmqtt.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_mqtt_common.o \
azure-iot-sdk-c/iothub_client/src/iothub_transport_ll_private.o \
azure-iot-sdk-c/iothub_client/src/version.o \
\
\
azure-iot-sdk-c/umqtt/src/mqtt_client.o \
azure-iot-sdk-c/umqtt/src/mqtt_codec.o \
azure-iot-sdk-c/umqtt/src/mqtt_message.o \
\
\
azure-iot-sdk-c/deps/parson/parson.o \
\
\
azure-iot-sdk-c/provisioning_client/src/prov_auth_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_device_ll_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_security_factory.o \
azure-iot-sdk-c/provisioning_client/src/iothub_security_factory.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_mqtt_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_mqtt_common.o \
azure-iot-sdk-c/provisioning_client/adapters/hsm_client_data.o \
azure-iot-sdk-c/provisioning_client/adapters/hsm_client_key.o \

COMPONENT_SRCDIRS := \
port/src \
azure-iot-sdk-c/certs \
azure-iot-sdk-c/c-utility/pal \
azure-iot-sdk-c/c-utility/pal/freertos \
azure-iot-sdk-c/c-utility/pal/lwip \
azure-iot-sdk-c/c-utility/src \
azure-iot-sdk-c/c-utility/adapters \
azure-iot-sdk-c/umqtt/src \
azure-iot-sdk-c/iothub_client/src \
azure-iot-sdk-c/deps/parson \
azure-iot-sdk-c/provisioning_client/src \
azure-iot-sdk-c/provisioning_client/adapters \

