#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BTN_PIN_R 28
#define BTN_PIN_G 26
#define LED_PIN_R 4
#define LED_PIN_G 6

volatile bool timer_ativo_r = false;
volatile bool timer_ativo_g = false;
volatile bool flag_r = false;
volatile bool flag_g = false;
repeating_timer_t timer_r;
repeating_timer_t timer_g;


bool timer_callback_r(repeating_timer_t *rt) {
    gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R)); 
    return true;
}


bool timer_callback_g(repeating_timer_t *rt) {
    gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G)); 
    return true;
}

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        if (gpio == BTN_PIN_R)
            flag_r = true;
        else if (gpio == BTN_PIN_G)
            flag_g = true;
    }
}



int main() {
    stdio_init_all();
    

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);
    
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_put(LED_PIN_G, 0);


    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
    

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
    
    while (true) {
        if (flag_r) {
            flag_r = false;
            if (!timer_ativo_r) {
                add_repeating_timer_ms(500, timer_callback_r, NULL, &timer_r);
                timer_ativo_r = true;
            } else {
                cancel_repeating_timer(&timer_r);
                timer_ativo_r = false;
                gpio_put(LED_PIN_R, 0);
            }
        }
        
        if (flag_g) {
            flag_g = false;
            if (!timer_ativo_g) {
                add_repeating_timer_ms(250, timer_callback_g, NULL, &timer_g);
                timer_ativo_g = true;
            } else {
                cancel_repeating_timer(&timer_g);
                timer_ativo_g = false;
                gpio_put(LED_PIN_G, 0);
            }
        }
        
        sleep_ms(10);
    }
}