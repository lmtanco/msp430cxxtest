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
#include "timer.hpp"

using namespace euma;

#pragma vector = TIMER1_A0_VECTOR
__interrupt void ISR_Timer1A0()
{
    static auto counter = 10;
    --counter;

    // A los 4s se cambia los leds.
    if (counter == 0) {
      port1.toggle_output(GPIO::BIT0 | GPIO::BIT6);
      counter = 10;
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
    timer1.set_clock_source(TIMER::SMCLK, TIMER::_8);
    timer1.enable_interrupt(TIMER::TACCR0_CCIFG);
    timer1.start(TIMER::UP,50000);

	// Sleep CPU
    __enable_interrupt();
	while(1);
}

