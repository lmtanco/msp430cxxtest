/*
 * watchdog.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: dianauma
 */

#include "watchdog.hpp"

namespace euma {

// Instancia del watchdog
WATCHDOG &the_watchdog = *new WATCHDOG{};

}


