
#include "nvs_flash.h"
// MQTT Stuff
#include "mqtt.h"
// Host
#include "host.h"
// Debug
#include "esp_log.h"

const char* Wifi_SSID = "RaindropZ"; //Shuhao's Galaxy Z Fold3 5G
const char* Wifi_Pass = "Raindropy";    //sils7884

// AWS Stuff
#define CONFIG_AWS_IOT_MQTT_TX_BUF_LEN 100
#define CONFIG_AWS_IOT_MQTT_RX_BUF_LEN 100
#define CONFIG_AWS_IOT_MQTT_NUM_SUBSCRIBE_HANDLERS i

const char* Mqtt_Broker_Url  = "mqtts://a2wfga7d77qb9w-ats.iot.us-east-2.amazonaws.com";
// Host object
host_t host;
void send_env_data(host_t* host);

void app_main(void)
{
  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  host.wifi_creds.Wifi_SSID = Wifi_SSID;
  host.wifi_creds.Wifi_Pass = Wifi_Pass;

  init_host(&host);

  mqtt_app_start(Mqtt_Broker_Url, &(host.mqtt_client));
  while(1)
  {
    send_env_data(&host);
    vTaskDelay(MS2TICK(500));
  }
}

void send_env_data(host_t* host)
{
  int msg_id;
  char buff[40];
  htu21_data_t env_data;
  if(host->htu21.msg_queue == NULL)
  {
    LOG_ERROR("MSG queue does not exist cannot send enviornment data.");
  }

  xQueuePeek(host->htu21.msg_queue, &env_data, 5);
  sprintf(buff,"Temputer:%.2f F\n Humidity:%.2f %%", env_data.temperature,env_data.humidity);
  //sprintf(buff, "{\"Temperature\": %.2f, \"Humidity\": %.2f}",env_data.temperature, env_data.humidity);
  msg_id = esp_mqtt_client_publish(host->mqtt_client, "Topic/env_data",
    buff, 0, 1, 0);
  LOG_PRINTF("Sent publish successful, msg_id=%d", msg_id);
}