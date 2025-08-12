/**
 * @file main.c
 * @brief Main entry point for the application.
 *
 * This file contains the main function which is the entry point of the application.
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

/**
 * @defgroup main Main Application
 * @brief Main application entry point.
 @{ */
#define HB_TASK_PRIORITY 5  /*!< Priority of the heartbeat task. */
#define SERIAL_TASK_PRIORITY 6 /*!< Priority of the serial task. */
/**
 @} */

/**
 * @brief Heartbeat task stack area made of 1024 bytes
 */
K_THREAD_STACK_DEFINE( hb_task_stack, 1024 );
/**
 * @brief Serial task stack area made of 1024 bytes
 */
K_THREAD_STACK_DEFINE( serial_task_stack, 1024 );

/**
 * @brief Heartbeat task ID.
 */
static k_tid_t  hb_task_id;
/**
 * @brief Serial task ID.
 */
static k_tid_t  serial_task_id;

/**
 * @brief Heartbeat task control block.
 */
static struct k_thread hb_task_tcb;
/**
 * @brief Serial task control block.
 */
static struct k_thread serial_task_tcb;

/**
 * @brief Heartbeat LED GPIO descriptor.
 */
const static struct gpio_dt_spec hb_led = GPIO_DT_SPEC_GET( DT_ALIAS( heartbeat ), gpios );

static bool init_rutine( void );
static void hb_task( void *p1, void *p2, void *p3 );
static void serial_task( void *p1, void *p2, void *p3 );

/**
 * @brief Main function of the application.
 *
 * This function initializes the heartbeat task and starts the Zephyr kernel.
 *
 * @return 0 on success, this function does not return.
 */
int main( void )
{
    /* Initialize all the hardware subsystem */
    if( init_rutine( ) ==  true ) {
        /* Register the heartbeat thread and run immediately */
	    hb_task_id = k_thread_create( &hb_task_tcb, hb_task_stack, K_THREAD_STACK_SIZEOF( hb_task_stack ),
                                	hb_task, (void*)&hb_led, NULL, NULL, HB_TASK_PRIORITY, 0, K_NO_WAIT );

        /* Register the serial thread and run immediately */
        serial_task_id = k_thread_create( &serial_task_tcb, serial_task_stack, K_THREAD_STACK_SIZEOF( serial_task_stack ),
                            	serial_task, NULL, NULL, NULL, SERIAL_TASK_PRIORITY, 0, K_NO_WAIT );
    }
    else {
        printk( "Initialization failed\n" );
        while( true );  // Loop indefinitely if initialization fails  
    }

    return 0; // This line will never be reached
}

/**
 * @brief Initializes the hardware and peripherals.
 *
 * This function initializes the GPIO subsystem and configures the heartbeat LED.
 *
 * @return true if initialization is successful, false otherwise.
 */
static bool init_rutine( void )
{
    /* Initialize the GPIO subsystem */
    if ( !gpio_is_ready_dt( &hb_led ) ) {
        printk( "Heartbeat LED GPIO is not ready\n" );
        return false;
    }

    /* Configure the heartbeat LED as output */
    if ( gpio_pin_configure_dt( &hb_led, GPIO_OUTPUT_ACTIVE ) < 0 ) {
        printk( "Failed to configure heartbeat LED GPIO\n" );
        return false;
    }

    /* Additional hardware initialization can be done here */
    printk( "Initialization complete\n" );
}

/**
 * @brief Heartbeat task function.
 *
 * This function runs in a loop, performing a heartbeat action (like toggling an LED)
 * at regular intervals.
 *
 * @param p1 Pointer to fir paramter where led descriptor is received.
 * @param p2 Pointer to the second parameter (not used).
 * @param p3 Pointer to the third parameter (not used).
 */
static void hb_task( void *p1, void *p2, void *p3 )
{
    struct gpio_dt_spec *led_spec = (struct gpio_dt_spec *)p1;

    while ( true ) {
        /* Toggle an LED or perform a heartbeat action */
        gpio_pin_toggle_dt( led_spec );
        k_msleep( 1000 ); // Sleep for 1 second
    }
}

/**
 * @brief Serial task function.
 *
 * This function runs in a loop, performing serial communication tasks.
 *
 * @param p1 Pointer to the first parameter (not used).
 * @param p2 Pointer to the second parameter (not used).
 * @param p3 Pointer to the third parameter (not used).
 */
static void serial_task( void *p1, void *p2, void *p3 )
{
    /* This task could handle serial communication, e.g., reading from UART */
    while ( true ) {
        /* Perform serial communication tasks */
        k_msleep( 500 ); // Sleep for 0.5 seconds
    }
}
