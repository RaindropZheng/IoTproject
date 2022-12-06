#ifndef __HOST_H__
#define __HOST_H__

/***************************************************************************************************/
/* Include Files */
/***************************************************************************************************/
// WiFi
#include "wifi.h"
// MQTT
#include "mqtt.h"
// I2C devices
#include "oled.h"
#include "htu21d.h"
#include "driver/i2c.h"
#include "i2c.h"
// Console
#include "driver/uart.h"
#include "serial.h"
#include "console.h"

typedef struct {
    unsigned char* Wifi_SSID;
    unsigned char* Wifi_Pass;
} wifi_creds_t;

typedef struct {
    console_t console;
    htu21_t htu21;
    ssd1306_t ssd1306;
    wifi_creds_t wifi_creds;
    esp_mqtt_client_handle_t mqtt_client;
} host_t;

void init_host(host_t* );

#endif /* __HOST_H__ */
