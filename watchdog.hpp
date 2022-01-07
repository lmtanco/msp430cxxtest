/*
 * watchdog2.hpp
 *
 *  Created on: Dec 30, 2021
 *      Author: Luis (lmtanco@uma.es)
 *
 *  Follows teachings of Dan Saks 
 */

#ifndef EUMA_WATCHDOG_HPP_
#define EUMA_WATCHDOG_HPP_

#include <type_traits>

#include "common.hpp"

// Namespace uma
namespace euma {

// Clase que empaqueta el watchdog
class WATCHDOG{
public:

    enum bits
    {
        WDTIS0 = 0x0001,
        WDTIS1 = 0x0002,
        WDTSSEL =0x0004,
        WDTCNTCL =0x0008,
        WDTTMSEL =0x0010,
        WDTNMI =0x0020,
        WDTNMIES =0x0040,
        WDTHOLD =0x0080,
        WDTPW =0x5A00,
    };
    // Una forma de hacer que el watchdog esté bien alineado
    // en memoria es redefinir el operador new
    static void* operator new(std::size_t) {
        return reinterpret_cast<void*>(0x120);
    }

    // Imprescindible que definamos constructor. Si no el compilador
    // llama a memset con 0s después del new! Eso nos crea un reset
    // puesto que no se puede poner 0s en WDTCTL sin poner el password.
    WATCHDOG(){}


    // Parar el watchdog.
    void stop(){
        _WDTCTL = bits::WDTPW | bits::WDTHOLD;
    }

    enum intervals{ _05ms, _8ms, _32ms };

    // WDT is clocked by fSMCLK (assumed 1MHz)
    void enable_timer_interrupt(intervals i) {

        // Configurar Intervalo
        switch (i) {
        case _05ms:
            _WDTCTL = bits::WDTPW+bits::WDTTMSEL+bits::WDTCNTCL+bits::WDTIS1;
            break;
        case _8ms:
            _WDTCTL = bits::WDTPW+bits::WDTTMSEL+bits::WDTCNTCL+bits::WDTIS0;
            break;
        case _32ms:
        default:
            _WDTCTL = bits::WDTPW+bits::WDTTMSEL+bits::WDTCNTCL;
            break;
        }
    }
    void enable_timer_interrupt()
    {
        // Esto se hace en IE1
        the_sfrs.enable_wdt_interrupt();
    }

    void disable_timer_interrupt()
    {
        // Esto se hace en IE1.
        the_sfrs.disable_wdt_interrupt();
    }
private:
    device_register16 _WDTCTL;
};

// Instancia para manejar el watchdog.
extern WATCHDOG &the_watchdog;

// Nos aseguramos de que el layout de la clase es el esperado:
// - El primer atributo no estático está a offset cero
// - Cada atributo tiene un offset mayor que el atributo justo antes que él.
static_assert(
        std::is_standard_layout<WATCHDOG>::value,
        "WATCHDOG isn't standard layout"
        );
}

#endif
