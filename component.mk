#
# Component Makefile
#
 
# Component configuration in preprocessor defines
CFLAGS += -DUSE_LWIP_SOCKET_FOR_AZURE_IOT -DHSM_TYPE_SYMM_KEY -DUSE_PROV_MODULE
CFLAGS += -Wno-unused-function -Wno-missing-braces -Wno-missing-field-initializers -Wno-unknown-pragmas -Wno-enum-compare -Wno-unused-variable -Wno-char-subscripts

COMPONENT_ADD_INCLUDEDIRS := \
azure-iot-sdk-c/c-utility/deps/azure-macro-utils-c/inc \
azure-iot-sdk-c/c-utility/deps/umock-c/inc \
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
azure-iot-sdk-c/uamqp/inc \
azure-iot-sdk-c/uamqp/inc/azure_uamqp_c \
azure-iot-sdk-c/deps/uhttp/inc \
azure-iot-sdk-c/deps/uhttp/inc/azure_uhttp_c \
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
azure-iot-sdk-c/c-utility/adapters/agenttime.o	\
azure-iot-sdk-c/c-utility/adapters/uniqueid_stub.o	\
azure-iot-sdk-c/c-utility/adapters/httpapi_compact.o	\
\
\
port/src/platform_esp_idf.o \
port/src/mbed_wait_api.o \
port/src/socketio_berkeley.o	\
azure-iot-sdk-c/c-utility/adapters/tlsio_mbedtls.o	\
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
azure-iot-sdk-c/c-utility/src/wsio.o \
azure-iot-sdk-c/c-utility/src/uws_client.o \
azure-iot-sdk-c/c-utility/src/uws_frame_encoder.o \
azure-iot-sdk-c/c-utility/src/utf8_checker.o \
azure-iot-sdk-c/c-utility/src/gb_rand.o \
azure-iot-sdk-c/c-utility/src/http_proxy_io.o \
azure-iot-sdk-c/c-utility/src/azure_base32.o \
azure-iot-sdk-c/c-utility/src/uuid.o \
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
azure-iot-sdk-c/iothub_client/src/iothub_transport_ll_private.o \
azure-iot-sdk-c/iothub_client/src/version.o \
azure-iot-sdk-c/iothub_client/src/iothubtransporthttp.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportmqtt.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportmqtt_websockets.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_mqtt_common.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_common.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportamqp_websockets.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportamqp.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_cbs_auth.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_common.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_connection.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_device.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_messenger.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportamqp_methods.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_telemetry_messenger.o \
azure-iot-sdk-c/iothub_client/src/iothubtransport_amqp_twin_messenger.o \
azure-iot-sdk-c/iothub_client/src/iothubtransportamqp_websockets.o \
azure-iot-sdk-c/iothub_client/src/uamqp_messaging.o \
azure-iot-sdk-c/iothub_client/src/message_queue.o \
\
\
azure-iot-sdk-c/umqtt/src/mqtt_client.o \
azure-iot-sdk-c/umqtt/src/mqtt_codec.o \
azure-iot-sdk-c/umqtt/src/mqtt_message.o \
\
\
azure-iot-sdk-c/uamqp/src/amqp_definitions.o \
azure-iot-sdk-c/uamqp/src/amqp_frame_codec.o \
azure-iot-sdk-c/uamqp/src/amqp_management.o \
azure-iot-sdk-c/uamqp/src/amqpvalue.o \
azure-iot-sdk-c/uamqp/src/amqpvalue_to_string.o \
azure-iot-sdk-c/uamqp/src/async_operation.o \
azure-iot-sdk-c/uamqp/src/cbs.o \
azure-iot-sdk-c/uamqp/src/connection.o \
azure-iot-sdk-c/uamqp/src/frame_codec.o \
azure-iot-sdk-c/uamqp/src/header_detect_io.o \
azure-iot-sdk-c/uamqp/src/link.o \
azure-iot-sdk-c/uamqp/src/message.o \
azure-iot-sdk-c/uamqp/src/message_receiver.o \
azure-iot-sdk-c/uamqp/src/message_sender.o \
azure-iot-sdk-c/uamqp/src/messaging.o \
azure-iot-sdk-c/uamqp/src/sasl_anonymous.o \
azure-iot-sdk-c/uamqp/src/saslclientio.o \
azure-iot-sdk-c/uamqp/src/sasl_frame_codec.o \
azure-iot-sdk-c/uamqp/src/sasl_mechanism.o \
azure-iot-sdk-c/uamqp/src/sasl_mssbcbs.o \
azure-iot-sdk-c/uamqp/src/sasl_plain.o \
azure-iot-sdk-c/uamqp/src/sasl_server_mechanism.o \
azure-iot-sdk-c/uamqp/src/session.o \
\
\
azure-iot-sdk-c/deps/uhttp/src/uhttp.o \
\
\
azure-iot-sdk-c/deps/parson/parson.o \
\
\
azure-iot-sdk-c/provisioning_client/src/prov_auth_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_device_ll_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_security_factory.o \
azure-iot-sdk-c/provisioning_client/src/iothub_security_factory.o \
azure-iot-sdk-c/provisioning_client/src/iothub_auth_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_mqtt_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_mqtt_ws_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_amqp_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_amqp_ws_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_http_client.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_mqtt_common.o \
azure-iot-sdk-c/provisioning_client/src/prov_transport_amqp_common.o \
azure-iot-sdk-c/provisioning_client/src/prov_sasl_tpm.o \
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
azure-iot-sdk-c/uamqp/src \
azure-iot-sdk-c/deps/uhttp/src \
azure-iot-sdk-c/iothub_client/src \
azure-iot-sdk-c/deps/parson \
azure-iot-sdk-c/provisioning_client/src \
azure-iot-sdk-c/provisioning_client/adapters \

