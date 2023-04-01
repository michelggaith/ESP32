#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#define BT 17

int display[] = {14,27,18,25,26,12,13};
int number[10][7]={
    {1,1,1,1,1,1,0}, //0
    {0,1,1,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,0,0}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,1,0,1,1}  //9
};


void init_hw(void)
{
    gpio_config_t io_config;                        //ESTRUCTURAS DE CONFIGURACION DIGITAL PARA PULSADOR; Genero todas las variables que quiero configurar
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pin_bit_mask = (1<<BT);
    io_config.pull_down_en =GPIO_PULLDOWN_ONLY;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;        //CONFIGURACION DE ESTRUCTURAS
    gpio_config(&io_config);

    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1<<display[0]) |
                             (1<<display[1]) |
                             (1<<display[2]) |
                             (1<<display[3]) |
                             (1<<display[4]) |
                             (1<<display[5]) |
                             (1<<display[6]) ;

    io_config.pull_down_en =GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;        //CONFIGURACION DE ESTRUCTURAS
    gpio_config(&io_config);
    /*gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BT, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BT, GPIO_PULLDOWN_ONLY);*/
}

void app_main() {       //Representa el void setup

    init_hw();
    int cnt = 0;
    for(int i=0; i<7; i++){
        gpio_set_level(display[i], number[0][i]);
    }
    while (1)
    {
        int state_bt = gpio_get_level(BT);
        for(int i=0; i<7; i++)
        {
            gpio_set_level(display[i], number[cnt][i]);
        }
        cnt = (state_bt && cnt<10) ? (cnt+1) : cnt;
        cnt = cnt > 9 ? 0 : cnt;
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    
}