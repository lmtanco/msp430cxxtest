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

using namespace euma;

#pragma vector = WDT_VECTOR
__interrupt void ISR_Watchdog()
{
    static auto counter = 50;
    --counter;

    // A los 3.2 s se cambia los leds.
    if (counter == 0) {
      port1.toggle_output(GPIO::BIT0 | GPIO::BIT6);
      counter = 50;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void ISR_Port1(void)
{
    if (port1.is_interrupt_pending(GPIO::BIT3)) {
        port1.clear_interrupt_flags(GPIO::BIT3);
        port1.toggle_output(GPIO::BIT0 | GPIO::BIT6);
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

    // Reset p1.3
    port1.reset_output(GPIO::BIT6);

    // Set p1.3 as pullup input
    port1.as_input(GPIO::BIT3, GPIO::PULLUP);

    // Set falling edge interrupt at p1.3
    port1.enable_interrupt(GPIO::BIT3, GPIO::FALLING);

    // Configure timer_interrupt
	the_watchdog.enable_timer_interrupt(WATCHDOG::_32ms);

	// Sleep CPU
	__low_power_mode_1();
}

