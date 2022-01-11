# msp430cxxtest

[Work in progress. Some clock, watchdog, GPIO and timer functionality]

First attempts at creating a c++ library for the TI msp430g2553 microcontroller. 

It is *not* meant to be a hardware abstraction layer - the hardware is not abstracted at all. 

The idea is only to have an "Easy to use correctly, difficult to use incorrectly (Scott Meyers)" software interface to the msp430g2553. 
With this in mind, the library hides the device registers as private members of classes. These are only accessed via class methods. 

There are several tests in the main/ folder. These are lines from one of them: 

```C++

    // stop watchdog timer
    the_watchdog.stop();

    // set DCO speed to 1MHz
    the_clock.set_dco(DCOFREQ::_1MHZ);

    // configure pins p1.6 and p1.0 as outputs.
    port1.as_output(GPIO::BIT0 | GPIO::BIT6);
    
    // Configure p1.3 as pullup input
    port1.as_input(GPIO::BIT3, GPIO::PULLUP);

    // Set falling edge interrupt at p1.3
    port1.enable_interrupt(GPIO::BIT3, GPIO::FALLING);

    // Configure timer_interrupt
    the_watchdog.enable_timer_interrupt(WATCHDOG::_32ms);

```
I am using Code Composer Studio version 11.1.0.00011, which ships with the TI MSP30 Compiler Version 18.1, which supports C++14.
I am testing the code with the MSP-EXP430G2ET board with an msp4302535 MCU. 

The library uses the ideas explained by Dan & Ben Saks in these two conferences: 
* [Memory-Mapped Devices as Objects - Dan Saks - CppCon 2020](https://www.youtube.com/watch?v=uwzuAGtAEFk)
* [Handling a Family of Hardware Devices with a Single Implementation - Ben Saks - CppCon 2021](https://www.youtube.com/watch?v=EM83l5NZ15c)

I was also very inspired to start this work by this other conference by Dan Saks: 
* [CppCon 2016: Dan Saks “extern c: Talking to C Programmers about C++”](https://www.youtube.com/watch?v=D7Sd8A6_fYU)




