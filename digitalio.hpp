/*
 * GPIO.hpp
 *
 *  Created on: Dec 31, 2021
 *      Author: Luis (lmtanco@uma.es)
 *
 *  Follows teachings of Dan Saks
 */

#ifndef EUMA_DIGITALIO_HPP_
#define EUMA_DIGITALIO_HPP_

#include "common.hpp"
#include <new>

namespace euma
{
// Puertos digitales del msp4302553
class GPIO
{
public:

    enum bits
    {
        BIT0 = 0x01,
        BIT1 = 0x02,
        BIT2 = 0x04,
        BIT3 = 0x08,
        BIT4 = 0x10,
        BIT5 = 0x20,
        BIT6 = 0x40,
        BIT7 = 0x80,
        ALL = 0xff
    };

    enum portnumber {_1, _2}; // msp4302553 solo tiene 2 puertos

    // Una forma de hacer que el watchdog esté bien alineado
    // en memoria es redefinir el operador new.
    // Además con el placement new podemos añadir el número de puerto
    static void* operator new(size_t, portnumber n)
    {
        auto address = 0x20 + n * 0x8;
        return reinterpret_cast<void*>(address);
    }

    // Configurar como salida, usa "OR" para modificar los bits
    void is_output(bits which = bits::ALL) {
        _PxDIR |= which;
    }
    // Poner salida a nivel alto "1", usa "OR" para modificar los bits.
    // Sólo modifica los que sean de salida.
    void set_output(bits which = bits::ALL)
    {
        _PxOUT |= (which & _PxDIR);
    }
    // Poner salida a nivel alto "1", usa "OR" para modificar los bits.
    // Sólo modifican los bits de which que sean de salida.
    void reset_output(bits which = bits::ALL)
    {
        _PxOUT &= ~(which & _PxDIR);
    }

private:
    device_register8 _PxIN;
    device_register8 _PxOUT;
    device_register8 _PxDIR;
    device_register8 _PxIFG;
    device_register8 _PxIES;
    device_register8 _PxIE;
    device_register8 _PxSEL;
    device_register8 _PxREN;
    // No estoy seguro de como mapear _PxSEL2 por que no es contiguo :(
    // Habrá que hacerlo con una clase aparte.
};

constexpr GPIO::bits operator|(GPIO::bits a, GPIO::bits b) {
    return static_cast<GPIO::bits>(static_cast<int>(a)|static_cast<int>(b));
}
constexpr GPIO::bits operator+(GPIO::bits a, GPIO::bits b) {
    return static_cast<GPIO::bits>(static_cast<int>(a)+static_cast<int>(b));
}
constexpr GPIO::bits operator&(GPIO::bits a, GPIO::bits b) {
    return static_cast<GPIO::bits>(static_cast<int>(a)&static_cast<int>(b));
}
constexpr GPIO::bits operator^(GPIO::bits a, GPIO::bits b) {
    return static_cast<GPIO::bits>(static_cast<int>(a)^static_cast<int>(b));
}
constexpr GPIO::bits operator~(GPIO::bits a) {
    return static_cast<GPIO::bits>(~static_cast<int>(a));
}


// Nos aseguramos de que el layout de la clase es el esperado:
// - El primer atributo no estático está a offset cero
// - Cada atributo tiene un offset mayor que el atributo justo antes que él.
static_assert(
        std::is_standard_layout<GPIO>::value,
        "GPIO isn't standard layout"
        );

// La comprobación anterior es necesaria pero no suficiente.
// Esto es especialmente necesario si hay más de un registro
// Sirve para comprobar que el compilador no ha metido relleno entre medias
// del los atributos.
static_assert(
        sizeof(GPIO) == 8 * sizeof(device_register8),
        "GPIO contains extra padding bytes"
        );

GPIO& port1 = *new (GPIO::portnumber::_1) GPIO{};
GPIO& port2 = *new (GPIO::portnumber::_2) GPIO{};
}

#endif /* GPIO_HPP_ */
