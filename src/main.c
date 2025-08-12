/**
 * @file main.c
 * @brief Main entry point for the application.
 *
 * This file contains the main function which is the entry point of the application.
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/**
 * @defgroup main Main Application
 * @brief Main application entry point.
 @{ */
#define HB_TASK_PRIORITY 5  /*!< Priority of the heartbeat task. */
/**
 @} */

/**
 * @brief Heartbeat task stack area made of 1024 bytes
 */
K_THREAD_STACK_DEFINE( hb_task_stack, 1024 );

/**
 * @brief Heartbeat task ID.
 */
static k_tid_t  hb_task_id;

/**
 * @brief Heartbeat task control block.
 */
static struct k_thread hb_task_tcb;

/**
 * @brief Heartbeat LED GPIO descriptor.
 */
const static struct gpio_dt_spec hb_led = GPIO_DT_SPEC_GET( DT_ALIAS( heartbeat ), gpios );

static void hb_task( void *p1, void *p2, void *p3 );

/**
 * @brief Main function of the application.
 *
 * This function initializes the heartbeat task and starts the Zephyr kernel.
 *
 * @return 0 on success, this function does not return.
 */
int main( void )
{
    /* Register the heartbeat thread and run immediately */
	hb_task_id = k_thread_create( &hb_task_tcb, hb_task_stack, K_THREAD_STACK_SIZEOF( hb_task_stack ),
                            	hb_task, NULL, NULL, NULL, HB_TASK_PRIORITY, 0, K_NO_WAIT );

    return 0; // This line will never be reached
}

/**
 * @brief Heartbeat task function.
 *
 * This function runs in a loop, performing a heartbeat action (like toggling an LED)
 * at regular intervals.
 *
 * @param p1 Pointer to the first parameter (not used).
 * @param p2 Pointer to the second parameter (not used).
 * @param p3 Pointer to the third parameter (not used).
 */
static void hb_task( void *p1, void *p2, void *p3 )
{
    /* Configure led0 and led1 as output */
    gpio_pin_configure_dt( &hb_led, GPIO_OUTPUT_ACTIVE );

    while ( true ) {
        /* Toggle an LED or perform a heartbeat action */
        gpio_pin_toggle_dt( &hb_led );
        k_msleep( 1000 ); // Sleep for 1 second
    }
}