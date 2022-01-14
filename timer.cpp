/*
 * timer.cpp
 *
 *  Created on: Jan 8, 2022
 *      Author: Luis (lmtanco@uma.es)
 */

#include <type_traits>
#include "timer.hpp"


namespace euma {

device_register16& timer1_traits::_TAxIV {*reinterpret_cast<device_register16*>(0x011E)};
device_register8&  timer1_traits::_P2SEL {*reinterpret_cast<device_register8*> (0x002E)};
device_register8&  timer1_traits::_P2SEL2{*reinterpret_cast<device_register8*> (0x0042)};
device_register8&  timer1_traits::_P2DIR {*reinterpret_cast<device_register8*> (0x002A)};

device_register16& timer0_traits::_TAxIV{ *reinterpret_cast<device_register16*>(0x012E)};
device_register8&  timer0_traits::_P1SEL{ *reinterpret_cast<device_register8*> (0x0026)};
device_register8&  timer0_traits::_P1SEL2{*reinterpret_cast<device_register8*> (0x0041)};
device_register8&  timer0_traits::_P1DIR{ *reinterpret_cast<device_register8*> (0x0022)};

TIMER_T<timer1_traits> & timer1 = *new TIMER_T<timer1_traits>{};
TIMER_T<timer0_traits> & timer0 = *new TIMER_T<timer0_traits>{};

// Nos aseguramos de que el layout de la clase es el esperado:
// - El primer atributo no estático está a offset cero
// - Cada atributo tiene un offset mayor que el atributo justo antes que él.
static_assert(
        std::is_standard_layout<TIMER_T<timer1_traits>>::value,
        "TIMER_T isn't standard layout"
        );



}




