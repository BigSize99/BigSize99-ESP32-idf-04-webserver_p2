#include "stdio.h"
#include "esp_log.h"
#include <driver/gpio.h>
#include <output_iot.h>

//=======================================================
//function create output
void output_io_create(gpio_num_t gpio_num)
{
    gpio_pad_select_gpio(gpio_num);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
}
//=======================================================
//function set output level (thiết lập chân output mức cao/thấp)
void output_io_set_level(gpio_num_t gpio_num, int level)
{
    gpio_set_level(gpio_num, level);
}
//=======================================================
//function get output level (truy xuất dữ liệu từ chân ouput)
int output_io_get_level(gpio_num_t gpio_num)
{
    return gpio_get_level(gpio_num);
}
//=======================================================
//function toggle (đảo trạng thái chân output)
void output_io_toggle(gpio_num_t gpio_num)
{
    int old_level = output_io_get_level(gpio_num);
    gpio_set_level(gpio_num, 1 - old_level);
}

