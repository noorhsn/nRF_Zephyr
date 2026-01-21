#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

/* Increase the sleep time from 100ms to 10 minutes  */
#define SLEEP_TIME_MS 10*60*1000

/* SW0_NODE is the devicetree node identifier for the "sw0" alias */
#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

/* LED0_NODE is the devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* Define the callback function */
void button_pressed_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
}

/* Define a variable of type static struct gpio_callback */
static struct gpio_callback button_cb_data;

int main(void)
{
	int ret;

	if (!device_is_ready(led.port)) {
		return -1;
	}

	if (!device_is_ready(button.port)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}
	/* Configure the interrupt on the button's pin */
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	/* Initialize the static struct gpio_callback variable   */
    gpio_init_callback(&button_cb_data, button_pressed_callback, BIT(button.pin)); 	

	/* Add the callback function by calling gpio_add_callback()   */
    gpio_add_callback(button.port, &button_cb_data);

	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
}