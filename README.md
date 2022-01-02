# msp430cxxtest

[Warning this is untested! I am programming this on my holidays and I do not have a microcontroller at hand, just the compiler!]

First attempts at creating a c++ hw abstraction layer for the TI msp430g2553 microcontroller. 

Using ideas from Dan Saks explained in this two conferences: 
* [Confefence 1: Memory-Mapped Devices as Objects - CppCon 2020](https://www.youtube.com/watch?v=uwzuAGtAEFk)
* [Conference 2: Writing better embedded Software - Embedded 2018](https://www.youtube.com/watch?v=3VtGCPIoBfs&t=3464s)

The idea is to have an "Easy to use correctly, difficult to use incorrectly" software interface to the msp430g2553. 

These are fragments of the main.cpp which tests the library: 

```C++

    // stop watchdog timer
    the_watchdog.stop();

    // set DCO speed to 1MHz
    the_clock.set_dco(DCOFREQ::_1MHZ);

    // configure port 1.6 and 1.0 as output.
    port1.as_output(GPIO::BIT0 | GPIO::BIT6);

    // set p1.0
    port1.set_output(GPIO::BIT0);
    
    // Set p1.3 as pullup input
    port1.as_input(GPIO::BIT3, GPIO::PULLUP);

    // Set falling edge interrupt at p1.3
    port1.set_interrupt(GPIO::BIT3, GPIO::FALLING, ISR_Port1);

    // Configure timer_interrupt
    the_watchdog.set_timer_interrupt(ISR_Watchdog, WATCHDOG::_32ms);

```
