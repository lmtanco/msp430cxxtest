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
#include "clock.hpp"

uint8_t events = 0;
#define EV1 0x01


using namespace euma;

__interrupt void ISR_Watchdog()
{
    static int counter = 10;
    --counter;

    // A los 3.2 s se cambia los leds.
    if (counter == 0) {
      events |= EV1;
    }
}


void main(void)
{
	// stop watchdog timer
    the_watchdog.stop();

    // set DCO speed to 1MHz
    the_clock.set_dco(DCOFREQ::_1MHZ);

    // configure port 1.6 and 1.0 as output.
    port1.as_output(GPIO::BIT0 | GPIO::BIT6);

    // set p1.0
    port1.set_output(GPIO::BIT0);

    // TODO !
    // port1.is_input(GPIO::BIT3, GPIO::PULLUP);
    // port1.set_interrupt(GPIO::BIT3, GPIO::FALLING, ISR_Port1);

    // Configure timer_interrupt
	the_watchdog.set_timer_interrupt(ISR_Watchdog,
	                                 WATCHDOG::_32ms);

	// Enable interrupts
	__enable_interrupt();

	// Loop forever
	while(1) {
	    if (events & EV1) {
	        events &= ~EV1;
	        port1.reset_output(GPIO::BIT0);
	        port1.set_output(GPIO::BIT6);

	    }
	}
}

