#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/adc.h>
#include <driver/ledc.h>
#define BT 17
#define LED 2

adc1_channel_t adc_pot = ADC_CHANNEL_6;
ledc_channel_config_t pwm;

void init_hw(void);

char display[] = {14,27,18,25,26,12,13};
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




void app_main() {       //Representa el void setup

    init_hw();
    int cnt = 0;
    for(int i=0; i<7; i++){
        gpio_set_level(display[i], number[0][i]);
    }
    while (1)
    {
        //CONTROL DE UN DISPLAY 7 segmentos
        int state_bt = gpio_get_level(BT);
        for(int i=0; i<7; i++)
        {
            gpio_set_level(display[i], number[cnt][i]);
        }
        cnt = (state_bt && cnt<10) ? (cnt+1) : cnt;
        cnt = cnt > 9 ? 0 : cnt;

    //CONTROL INTENSIDAD DE LED CON POTE, ADC y PWM
        int pot = adc1_get_raw(adc_pot);
        printf("ADC: %d\n", pot);
        ledc_set_duty(pwm.speed_mode, pwm.channel, pot);
        ledc_update_duty(pwm.speed_mode, pwm.channel);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    
}

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

    /*CONFIGURACION DEL ADC*/
    adc1_config_width(ADC_WIDTH_BIT_12);        // CONFIGURACION DE LA CANTIDAD DE BITS DEL ADC
    adc1_config_channel_atten(adc_pot, ADC_ATTEN_DB_11); //CONFIGURACION DE LA ATENUACION DEL ADC
    
    /*DESDE ACA SE GENERA LA CONFIGURACION DEL PWM*/
    ledc_timer_config_t timer = {
        .duty_resolution = LEDC_TIMER_12_BIT,
        .freq_hz = 1000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);
    pwm.channel = LEDC_CHANNEL_0;
    pwm.duty = 0;
    pwm.gpio_num = LED;
    pwm.hpoint = 0;
    pwm.timer_sel = LEDC_TIMER_0;
    pwm.speed_mode = LEDC_HIGH_SPEED_MODE;

    ledc_channel_config(&pwm);

}
