/*
 * digitalio.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: dianauma
 */

#include "digitalio.hpp"

namespace euma {

// Definición de la dirección donde está mapeado P1SEL2 (0x041)
device_register8& port1_traits::_PxSEL2{*reinterpret_cast<device_register8*>(0x041)};

// Definición de la dirección donde está mapeado P2SEL2 (0x042)
device_register8& port2_traits::_PxSEL2{*reinterpret_cast<device_register8*>(0x042)};

// Definición del puerto 1
GPIO_P<port1_traits>& port1 = *new GPIO_P<port1_traits>{};

// Definición del puerto 2
GPIO_P<port2_traits>& port2 = *new GPIO_P<port2_traits>{};


}



