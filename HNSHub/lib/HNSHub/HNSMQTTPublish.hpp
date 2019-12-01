#ifndef HNS_MQTT_PUBLISH_HPP_
#define HNS_MQTT_PUBLISH_HPP_
#include "../HNSCore/HNSError.h"
#include "../HNSCore/HNSRouting.h"

#include <string>

using namespace std;




res_t HNSMQTTPublish_publish(string topic, string payload);
res_t HNSMQTTPublish_init(const char* host);
res_t HNSMQTTPublish_publishAll();
res_t HNSMQTTPublish_publishDevice(address_t address);

#endif /*HNS_MQTT_PUBLISH_HPP_*/
