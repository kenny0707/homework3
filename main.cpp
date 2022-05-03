#include "mbed.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "stm32l475e_iot01_accelero.h"
#include "erpc_simple_server.h"
#include "erpc_basic_codec.h"
#include "erpc_crc16.h"
#include "UARTTransport.h"
#include "DynamicMessageBufferFactory.h"
#include "controller_server.h"
using namespace std::chrono;

int16_t pDataXYZ[3] = {0};
bool measure = false;

EventQueue queue;
Thread t;
WiFiInterface *wifi;
const char* topic = "Mbed";

void stop() {
    measure = false;
}

void start() {
    measure = true;
}

ep::UARTTransport uart_transport(D1, D0, 9600);
ep::DynamicMessageBufferFactory dynamic_mbf;
erpc::BasicCodecFactory basic_cf;
erpc::Crc16 crc16;
erpc::SimpleServer rpc_server;
SwitchService_service control_service;

void publish_message(MQTT::Client<MQTTNetwork, Countdown>* client) {
    while (measure) {
        MQTT::Message message;
        char buff[100];
        BSP_ACCELERO_AccGetXYZ(pDataXYZ);
        sprintf(buff, " %d %d %d ", pDataXYZ[0], pDataXYZ[1], pDataXYZ[2]);
        message.qos = MQTT::QOS0;
        message.retained = false;
        message.dup = false;
        message.payload = (void*) buff;
        message.payloadlen = strlen(buff) + 1;
        int rc = client->publish(topic, message);
        printf("%s\r\n", buff);
        ThisThread::sleep_for(100ms);
    }
}

int main() {  
    BSP_ACCELERO_Init();
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
            printf("ERROR: No WiFiInterface found.\r\n");
            return -1;
    }
    printf("\nConnecting to %s...\r\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
            printf("\nConnection error: %d\r\n", ret);
            return -1;
    }
    const char* host = "172.20.10.6";
    const int port=1883;
    printf("Connecting to TCP network...\r\n");
    printf("address is %s/%d\r\n", host, port);
    NetworkInterface* net = wifi;
    MQTTNetwork mqttNetwork(net);
    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork); 
    int rc = mqttNetwork.connect(host, port);
    if (rc != 0) {
            printf("Connection error.");
            return -1;
    }
    printf("Successfully connected!\r\n");
    queue.call_every(100ms, &publish_message, &client);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "Mbed";
    if ((rc = client.connect(data)) != 0){
            printf("Fail to connect MQTT\r\n");
    }
    uart_transport.setCrc16(&crc16);
    rpc_server.setTransport(&uart_transport);
    rpc_server.setCodecFactory(&basic_cf);
    rpc_server.setMessageBufferFactory(&dynamic_mbf);
    rpc_server.addService(&control_service);
    rpc_server.run();
    return 0;
}