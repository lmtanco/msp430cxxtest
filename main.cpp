/*
 * main.cpp
 *
 *  Created on: Dec 30, 2021
 *      Author: Luis (lmtanco@uma.es)
 *
 *      Stops watchdog
 *      Sets  watchdog as
 */

#include "watchdog.hpp"
#include "digitalio.hpp"

using namespace euma;

void ISR_Watchdog()
{
    static int counter = 10;
    --counter;

    // A los 3.2 s se cambia los leds.
    if (counter == 0) {
        port1.reset_output(GPIO::BIT0);
        port1.set_output(GPIO::BIT6);
    }
}


void main(void)
{
	// stop watchdog timer
    the_watchdog.stop();

    // configure port 1.6 and 1.0 as output.
    port1.is_output(GPIO::BIT0 | GPIO::BIT6);

    // set p1.0
    port1.set_output(GPIO::BIT0);

    // Configure timer_interrupt
	the_watchdog.set_timer_interrupt(ISR_Watchdog,
	                                 WATCHDOG::intervals::_32ms);

	// Enable interrupts
	__enable_interrupt();

	// Loop forever
	while(1);

}

