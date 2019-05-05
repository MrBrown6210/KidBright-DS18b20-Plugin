#ifndef __DS_H__
#define __DS_H__

#include "driver.h"
#include "device.h"

// #include "esp32-ds18b20-example/components/esp32-ds18b20/include/ds18b20.h"
// #include "esp32-ds18b20-example/components/esp32-owb/include/owb.h"
// #include "esp32-ds18b20-example/components/esp32-owb/include/owb_gpio.h"
// #include "esp32-ds18b20-example/components/esp32-owb/include/owb_rmt.h"

// #define GPIO_DS18B20_0       (CONFIG_ONE_WIRE_GPIO)
#define GPIO_DS18B20_0       (18)
#define MAX_DEVICES          (8)
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)
#define SAMPLE_PERIOD        (1000)   // milliseconds

#define DS_POLLING_MS         1000

class DS : public Device {
    private:
        enum { s_detect } state;
        TickType_t tickcnt, polling_tickcnt;

    public:
        // constructor
        DS();
        // override
        void init(void);
        void process(Driver *drv);
        int prop_count(void);
        bool prop_name(int index, char *name);
        bool prop_unit(int index, char *unit);
        bool prop_attr(int index, char *attr);
        bool prop_read(int index, char *value);
        bool prop_write(int index, char *value);
        // method
        char* random();
        double getTemp();
};

#endif
