/*
 * common.hpp
 *
 *  Created on: Dec 30, 2021
 *      Author: Luis (lmtanco@uma.es)
 */

#ifndef EUMA_COMMON_HPP_
#define EUMA_COMMON_HPP_

#include <intrinsics.h>
#include <cstdint>

// Namespace uma
namespace euma {

// Alias de c++, igual que un typedef pero se lee mejor!
using device_register16 = uint16_t volatile;
using device_register8 = uint8_t volatile;
using pointer_to_ISR = void (*)();

// Sólo válida para msp430g2x53 y 13
#if defined (__MSP430G2553__)

// Clase que empaqueta todos los registros especiales (SFR)

class SFR {
public:

    enum bits {
        WDTIE = 0x01,
        OFIE = 0x02,
        NMIIE = 0x10,
        ACCVIE = 0x20,

        UCA0RXIE=0x01,
        UCA0TXIE=0x02,
        UCB0RXIE=0x04,
        UCB0TXIE=0x08,

        WDTIFG=0x01,
        OFIFG=0x02,
        PORIFG=0x04,
        RSTIFG=0x08,
        NMIIFG=0x10,

        UCA0RXIFG=0x01,
        UCA0TXIFG=0x02,
        UCB0RXIFG=0x04,
        UCB0TXIFG=0x08
    };

    void enable_wdt_interrupt() {
        _IFG1 &= ~(bits::WDTIFG);
        _IE1 |= bits::WDTIE;
    }

    void disable_wdt_interrupt() {
        _IFG1 &= ~(bits::WDTIFG);
        _IE1 &= ~(bits::WDTIE);
    }

    // Una forma de hacer que el watchdog esté bien alineado
     // en memoria es redefinir el operador new.
     static void* operator new(std::size_t) {
         return reinterpret_cast<void*>(0x000);
     }
private:
    device_register8 _IE1;
    device_register8 _IE2;
    device_register8 _IFG1;
    device_register8 _IFG2;
};

SFR& the_sfrs = *new SFR{};

// Nos aseguramos de que el layout de la clase es el esperado:
// - El primer atributo no estático está a offset cero
// - Cada atributo tiene un offset mayor que el atributo justo antes que él.
static_assert(
        std::is_standard_layout<SFR>::value,
        "SFR isn't standard layout"
        );

// La comprobación anterior es necesaria pero no suficiente.
// Esto es especialmente necesario si hay más de un registro
// Sirve para comprobar que el compilador no ha metido relleno entre medias
// del los atributos.
static_assert(
        sizeof(SFR) == 4 * sizeof(device_register8),
        "SFR contains extra padding bytes"
        );

// Tabla para que las clases de los periféricos guarden sus ISRs

class IVT {
public:
    enum number {
          begin=18,
          PORT1=begin,
          PORT2,
          RESERVED,
          ADC,
          USCI_TX,
          USCI_RX,
          TIMER0_CCR1,
          TIMER0_CCR2 = TIMER0_CCR1,
          TIMER0_OVERFLOW = TIMER0_CCR1,
          TIMER0_CCR0,
          WATCHDOG,
          COMPARATOR,
          TIMER1_CCR1,
          TIMER1_CCR2 = TIMER1_CCR1,
          TIMER1_OVERFLOW = TIMER1_CCR1,
          TIMER1_CCR0,
          NMI,
          RESET,
          end
    };
    static_assert(static_cast<int>(RESET) == 31,"Wrong number of vectors in table");

    // Método para usar la tabla sólo con los enums de arriba
    pointer_to_ISR & operator[](number n) {
        return table[n];
    }

private:
    pointer_to_ISR table[end-begin];
};

// Declaramos referencia a la tabla en la dirección del primer vector (PORT1)
IVT& ivt_table = *reinterpret_cast<IVT*>(0xFFE4);

#endif

}

#endif /* COMMON_HPP_ */
