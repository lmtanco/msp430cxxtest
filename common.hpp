/*
 * common.hpp
 *
 *  Created on: Dec 30, 2021
 *      Author: Luis (lmtanco@uma.es)
 */

#ifndef EUMA_COMMON_HPP_
#define EUMA_COMMON_HPP_

// Esto es lo único que necesitamos de <msp4302553.h>
#define TRAPINT_VECTOR          (0 * 1u)                     /* 0xFFE0 TRAPINT */
#define PORT1_VECTOR            (2 * 1u)                     /* 0xFFE4 Port 1 */
#define PORT2_VECTOR            (3 * 1u)                     /* 0xFFE6 Port 2 */
#define ADC10_VECTOR            (5 * 1u)                     /* 0xFFEA ADC10 */
#define USCIAB0TX_VECTOR        (6 * 1u)                     /* 0xFFEC USCI A0/B0 Transmit */
#define USCIAB0RX_VECTOR        (7 * 1u)                     /* 0xFFEE USCI A0/B0 Receive */
#define TIMER0_A1_VECTOR        (8 * 1u)                     /* 0xFFF0 Timer0_A CC1, TA0 */
#define TIMER0_A0_VECTOR        (9 * 1u)                     /* 0xFFF2 Timer0_A CC0 */
#define WDT_VECTOR              (10 * 1u)                    /* 0xFFF4 Watchdog Timer */
#define COMPARATORA_VECTOR      (11 * 1u)                    /* 0xFFF6 Comparator A */
#define TIMER1_A1_VECTOR        (12 * 1u)                    /* 0xFFF8 Timer1_A CC1-4, TA1 */
#define TIMER1_A0_VECTOR        (13 * 1u)                    /* 0xFFFA Timer1_A CC0 */
#define NMI_VECTOR              (14 * 1u)                    /* 0xFFFC Non-maskable */
#define RESET_VECTOR            (15 * 1u)                    /* 0xFFFE Reset [Highest Priority] */

#include <intrinsics.h>
#include <cstdint>
#include <type_traits>

// Namespace uma
namespace euma {

// Alias de c++, igual que un typedef pero se lee mejor!
using device_register16 = uint16_t volatile;
using device_register8 = uint8_t volatile;

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

extern SFR& the_sfrs;

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

#endif

}
//
#endif /* COMMON_HPP_ */
