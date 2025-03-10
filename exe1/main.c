#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BTN_PIN_R 28
#define LED_PIN_R 4

volatile bool timer_ativo = false;
volatile bool btn = false;
repeating_timer_t timer;

bool timer_callback(repeating_timer_t *rt) {
    gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R)); 
    return true; 
}

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        btn = true;
    }
}

int main() {
    stdio_init_all();
    

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0); 


    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);
    

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    
    while (true) {
        if (btn) {
            btn = false; 
            
            if (!timer_ativo) {
                add_repeating_timer_ms(500, timer_callback, NULL, &timer);
                timer_ativo = true;
            } else {
                cancel_repeating_timer(&timer);
                timer_ativo = false;
                gpio_put(LED_PIN_R, 0);
            }
        }
        sleep_ms(10); 
    }
}
