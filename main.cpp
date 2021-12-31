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

using namespace euma;

void ISR_Watchdog()
{
    // Does nothing for the mo
    __no_operation();
}


void main(void)
{
	// stop watchdog timer
    the_watchdog.stop();

    // Configure timer_interrupt
	the_watchdog.set_timer_interrupt(ISR_Watchdog,
	                                 WATCHDOG::intervals::_32ms);

	// Enable interrupts
	__enable_interrupt();

	// Loop forever
	while(1);

}

