#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BTN_PIN_R 28
#define BTN_PIN_G 26
#define LED_PIN_R 4
#define LED_PIN_G 6

volatile bool btn_r = false;
volatile bool btn_g = false;
volatile bool timer_ativo_r = false;
volatile bool timer_ativo_g = false;

bool timer_callback_r(repeating_timer_t *rt)
{
    timer_ativo_r = true;
    return true;
}
bool timer_callback_g(repeating_timer_t *rt)
{
    timer_ativo_g = true;
    return true;
}

void btn_callback(uint gpio, uint32_t events)
{
    if (events & GPIO_IRQ_EDGE_FALL)
    {
        if (gpio == BTN_PIN_R)
            btn_r = !btn_r;
        if (gpio == BTN_PIN_G)
            btn_g = !btn_g;
    }
}

int main()
{
    repeating_timer_t timer;
    repeating_timer_t timer2;

    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_put(LED_PIN_G, 0);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    if (!add_repeating_timer_ms(500, timer_callback_r, NULL, &timer))
    {
        printf("Failed to add timer/n");
    }

    if (!add_repeating_timer_ms(250, timer_callback_g, NULL, &timer2))
    {
        printf("Failed to add timer/n");
    }
    while (true)
    {
        if (btn_r)
        {
            if (timer_ativo_r)
            {
                gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
                timer_ativo_r = false;
            }
        }
        else
        {
            gpio_put(LED_PIN_R, 0);
        }
        if (btn_g)
        {
            if (timer_ativo_g)
            {
                gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G));
                timer_ativo_g = false;
            }
        }
        else
        {
            gpio_put(LED_PIN_G, 0);
        }
    }
}
