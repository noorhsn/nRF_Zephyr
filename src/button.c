#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include "button.h"

/* Use the devicetree alias "sw0" if present */
#define BUTTON_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON_NODE, gpios, {0});

int button_init(void)
{
    /* If the alias/node isn't present, gpio_is_ready_dt will be false */
    if (!gpio_is_ready_dt(&button)) {
        return -ENOTSUP;
    }

    return gpio_pin_configure_dt(&button, GPIO_INPUT);
}

bool button_pressed(void)
{
    if (!gpio_is_ready_dt(&button)) {
        return false;
    }

    int val = gpio_pin_get_dt(&button);
    /* Many boards use active-low buttons; treat 0 as pressed */
    return (val == 0);
}