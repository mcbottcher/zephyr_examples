#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>
#include "led_indicator/led_indicator.h"

#define AWAKE_DURATION_MS 5000
#define DEBOUNCE_MS       20

static const struct gpio_dt_spec toggle_led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_NODELABEL(user_button), gpios);

static struct gpio_callback btn_cb_data;
static struct pm_policy_latency_request awake_req;
static bool awake_req_active;

static void button_work_fn(struct k_work *work);
static void release_awake_constraint(struct k_work *work);

/* A delyable work object is placed on the system work queue after a given time
   It uses a timer to count before it is placed in the queue.

   Basically an interrupt is set on the hardware timer assigned to zephyr
   for whatever the next timed event is (e.g. k_sleep), so that the kernel
   can go back and check when this is done.

   Since this is a hardware interrupt, if nothing else is happening the idle
   thread may also go into WFI state.
   */
static K_WORK_DELAYABLE_DEFINE(button_work, button_work_fn);
static K_WORK_DELAYABLE_DEFINE(sleep_work, release_awake_constraint);

/* Runs in ISR context — keep minimal. */
static void button_isr(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);
	k_work_reschedule(&button_work, K_MSEC(DEBOUNCE_MS));
}

static void button_work_fn(struct k_work *work)
{
	ARG_UNUSED(work);

	gpio_pin_toggle_dt(&toggle_led);
	printk("Button pressed — toggle LED changed\n");

	/* A latency of 0 means only PM_STATE_ACTIVE qualifies, preventing
	 * any suspend state until the constraint is released. */
	if (!awake_req_active) {
		pm_policy_latency_request_add(&awake_req, 0);
		awake_req_active = true;
		pm_device_action_run(LED_INDICATOR_DEV, PM_DEVICE_ACTION_RESUME);
		printk("Awake constraint set — staying awake for %d s\n", AWAKE_DURATION_MS / 1000);
	}

	/* Restart the countdown on every press. */
	k_work_reschedule(&sleep_work, K_MSEC(AWAKE_DURATION_MS));
}

static void release_awake_constraint(struct k_work *work)
{
	ARG_UNUSED(work);
	pm_policy_latency_request_remove(&awake_req);
	awake_req_active = false;
	pm_device_action_run(LED_INDICATOR_DEV, PM_DEVICE_ACTION_SUSPEND);
	printk("Awake constraint released — system may suspend\n");
}

int main(void)
{
	if (!device_is_ready(LED_INDICATOR_DEV)) {
		printk("LED indicator device not ready\n");
		return 0;
	}

	if (!gpio_is_ready_dt(&toggle_led)) {
		printk("Toggle LED GPIO not ready\n");
		return 0;
	}
	gpio_pin_configure_dt(&toggle_led, GPIO_OUTPUT_INACTIVE);

	if (!gpio_is_ready_dt(&btn)) {
		printk("Button GPIO not ready\n");
		return 0;
	}
	gpio_pin_configure_dt(&btn, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_init_callback(&btn_cb_data, button_isr, BIT(btn.pin));
	gpio_add_callback_dt(&btn, &btn_cb_data);

	printk("System-managed PM example ready\n");
	printk("Press button to toggle LED.\n");
	printk("System suspends after %d s of inactivity.\n", AWAKE_DURATION_MS / 1000);

	return 0;
}
