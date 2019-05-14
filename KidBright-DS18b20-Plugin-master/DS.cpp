#include <stdio.h>
#include <string.h>
#include <math.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "DS.h"

// #include "gpio.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

// #include "ds18b20.h"

#define DS_GPIO GPIO_NUM_18

int init=0;

void ds18b20_send(char bit){
  gpio_set_direction(DS_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(DS_GPIO,GPIO_NUM_0);
  ets_delay_us(5);
  if(bit==1)gpio_set_level(DS_GPIO,1);
  ets_delay_us(80);
  gpio_set_level(DS_GPIO,1);
}
// Reads one bit from bus
unsigned char ds18b20_read(void){
  unsigned char PRESENCE=0;
  gpio_set_direction(DS_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(DS_GPIO,0);
  ets_delay_us(2);
  gpio_set_level(DS_GPIO,1);
  ets_delay_us(15);
  gpio_set_direction(DS_GPIO, GPIO_MODE_INPUT);
  if(gpio_get_level(DS_GPIO)==1) PRESENCE=1; else PRESENCE=0;
  return(PRESENCE);
}
// Sends one byte to bus
void ds18b20_send_byte(char data){
  unsigned char i;
  unsigned char x;
  for(i=0;i<8;i++){
    x = data>>i;
    x &= 0x01;
    ds18b20_send(x);
  }
  ets_delay_us(100);
}
// Reads one byte from bus
unsigned char ds18b20_read_byte(void){
  unsigned char i;
  unsigned char data = 0;
  for (i=0;i<8;i++)
  {
    if(ds18b20_read()) data|=0x01<<i;
    ets_delay_us(15);
  }
  return(data);
}
// Sends reset pulse
unsigned char ds18b20_RST_PULSE(void){
  unsigned char PRESENCE;
  gpio_set_direction(DS_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(DS_GPIO,0);
  ets_delay_us(500);
  gpio_set_level(DS_GPIO,1);
  gpio_set_direction(DS_GPIO, GPIO_MODE_INPUT);
  ets_delay_us(30);
  if(gpio_get_level(DS_GPIO)==0) PRESENCE=1; else PRESENCE=0;
  ets_delay_us(470);
  if(gpio_get_level(DS_GPIO)==1) PRESENCE=1; else PRESENCE=0;
  return PRESENCE;
}
// Returns temperature from sensor
float ds18b20_get_temp(void) {
  if(init==1){
    unsigned char check;
    char temp1=0, temp2=0;
      check=ds18b20_RST_PULSE();
      if(check==1)
      {
        ds18b20_send_byte(0xCC);
        ds18b20_send_byte(0x44);
        vTaskDelay(750 / portTICK_RATE_MS);
        check=ds18b20_RST_PULSE();
        ds18b20_send_byte(0xCC);
        ds18b20_send_byte(0xBE);
        temp1=ds18b20_read_byte();
        temp2=ds18b20_read_byte();
        check=ds18b20_RST_PULSE();
        float temp=0;
        temp=(float)(temp1+(temp2*256))/16;
        return temp;
      }
      else{return 0;}

  }
  else{return 0;}
}
void ds18b20_init(int GPIO){
  gpio_pad_select_gpio(DS_GPIO);
  init=1;
}

const int DS_PIN = 18;

float current_temp = 0;

DS::DS() {
  polling_ms = DS_POLLING_MS;
}

void DS::init(void) {
  state = s_detect;
  initialized = true;
  ds18b20_init(DS_PIN);

  char test[] = "init temp";
  printf("%s\n", test);

  // while (1) {
  //   printf("Temperature: %0.1f\n",ds18b20_get_temp());
  //   current_temp = ds18b20_get_temp();
  //   vTaskDelay(10 / portTICK_PERIOD_MS);
  // }

  // OneWireBus * owb;
  // owb_rmt_driver_info rmt_driver_info;
  // rmt_channel_t a = RMT_CHANNEL_1;
  // rmt_channel_t b = RMT_CHANNEL_0;
  // printf('xxx'.c_str());
  // string sName1 = EnumToString(RMT_CHANNEL_0);
  // printf(sName1.c_str());
  // owb = owb_rmt_initialize(&rmt_driver_info, GPIO_DS18B20_0, RMT_CHANNEL_1, RMT_CHANNEL_0);
  // owb = owb_rmt_initialize(&rmt_driver_info, GPIO_DS18B20_0, 0, 1);
  // owb_use_crc(owb, true);  // enable CRC check for ROM code
}

int DS::prop_count(void) {
  // not supported
  return 0;
}

bool DS::prop_name(int index, char *name) {
  // not supported
  return false;
}

bool DS::prop_unit(int index, char *unit) {
  // not supported
  return false;
}

bool DS::prop_attr(int index, char *attr) {
  // not supported
  return false;
}

bool DS::prop_read(int index, char *value) {
  // not supported
  return false;
}

bool DS::prop_write(int index, char *value) {
  // not supported
  return false;
}

void DS::process(Driver *drv) {
//  char buffer[64];
//  sprintf(buffer, "%lldms\n", (esp_timer_get_time() / 1000));
//  uart_write_bytes(UART_NUM_0, (const char*)buffer, strlen(buffer));
}


char* DS::random() {
    return "nat-random";
}

double DS::getTemp() {
  float temp = ds18b20_get_temp();
  if (temp >= 4000 && temp <= 4095.5) {
    temp = temp - 4095.5;
  }
  printf("%0.6f\n", temp);
  double dValue(0.0);
  dValue = static_cast<double>(temp);
  // printf("%.2lf\n", dValue);
  return dValue;
}

