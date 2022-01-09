/*
 * timer.cpp
 *
 *  Created on: Jan 8, 2022
 *      Author: Luis (lmtanco@uma.es)
 */

#include "timer.hpp"

namespace euma {

device_register16& timer1_A3_traits::_TAxIV{*reinterpret_cast<device_register16*>(0x011E)};

device_register16& timer0_A3_traits::_TAxIV{*reinterpret_cast<device_register16*>(0x012E)};

TIMER_T<timer1_A3_traits>& timer1 = *new TIMER_T<timer1_A3_traits>{};
TIMER_T<timer0_A3_traits>& timer0 = *new TIMER_T<timer0_A3_traits>{};
}




