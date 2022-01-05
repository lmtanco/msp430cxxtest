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
//#include <array>
//#include <algorithm>

namespace euma
{

// Clase para definir constantes para usar los puertos digitales del msp4302553
class GPIO {
public:

    // Bit
    enum bits {
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

    // Tipo de entrada
    enum input_type{
        NOINTERNALRES,
        PULLUP,
        PULLDOWN
    };

    // Tipo de interrupción
    enum edge {RISING, FALLING};

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

// Particularidades del puerto 1
struct port1_traits {
    static constexpr std::uintptr_t address{0x20};
    static device_register8& PxSEL2; //0x041;

};

//Particularidades del puerto 2
struct port2_traits {
    static constexpr std::uintptr_t address{0x28};
    static device_register8& PxSEL2; //0x042;
};


// Puertos digitales del msp4302553.
template <typename port_traits>
class GPIO_P: GPIO
{
public:

    // Una forma de hacer que el watchdog esté bien alineado
    // en memoria es redefinir el operador new.
    static void* operator new(std::size_t)
    {
       return reinterpret_cast<void*>(port_traits::address);
    }

    // Configurar como salida, usa "OR" para modificar los bits
    void as_output(bits which = bits::ALL) {
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

    // Configurar como entrada, usa AND para modificar los bits
    void as_input(bits which = bits::ALL,
                  input_type what = input_type::NOINTERNALRES) {

        _PxDIR &= ~(which);

        switch (what) {
        case NOINTERNALRES: break;
        case PULLUP:
            _PxREN |= which;
            _PxOUT |= which;
            break;
        case PULLDOWN:
            _PxREN |= which;
            _PxOUT &= ~(which);
            break;
        }
    }

    // Debería poder comprobarse más de una o no?
    bool is_interrupt_pending(bits which) {
        return static_cast<bool>(_PxIFG & _PxIE & which);
    }

    // Borrar flags de interrupcion;
    void clear_interrupt_flags(bits which) {
        _PxIE &= ~(which);
    }

    // Habilitar interrupción en PxIE y PxIES
    // También B¡borra los flags de interrupción
    // _PxIFG indicados en which.
    void enable_interrupt(bits which, edge type) {
        _PxIFG &= ~(which);
         _PxIE |= which;
         if (type == RISING) {
             _PxIES &= ~(which);
         }
         else {
             _PxIES |= which;
         }
    }

    // Borar todos los registros ¿útil?
    // void reset_all_registers() {
    //    std::fill(std::begin(a),std::end(a), 0);
    // }


private:
    // Descomentar si se quiere implemetar reset_all_registers
    // También: todos los registros serían r._PxIN, r._PxOUT...
    // union {
    // struct {
    device_register8 _PxIN;
    device_register8 _PxOUT;
    device_register8 _PxDIR;
    device_register8 _PxIFG;
    device_register8 _PxIES;
    device_register8 _PxIE;
    device_register8 _PxSEL;
    device_register8 _PxREN;
    // _PxSEL2 no está contiguo en memoria, por eso se lo
    // proporciona port_traits con port_traits::PxSEL2
     //    } r;
    //    std::array<device_register8,8> a;
    //};

};

// Nos aseguramos de que el layout de la clase es el esperado:
// - El primer atributo no estático está a offset cero
// - Cada atributo tiene un offset mayor que el atributo justo antes que él.
static_assert(
        std::is_standard_layout<GPIO_P<port1_traits>>::value,
        "GPIO isn't standard layout"
        );

// La comprobación anterior es necesaria pero no suficiente.
// Esto es especialmente necesario si hay más de un registro
// Sirve para comprobar que el compilador no ha metido relleno entre medias
// del los atributos.
static_assert(
        sizeof(GPIO_P<port1_traits>) == 8 * sizeof(device_register8),
        "GPIO contains extra padding bytes"
        );
// Referencia al puerto 1 que se usa en los programas.
extern GPIO_P<port1_traits>& port1;

// Referencia al puerto 2 que se usa en los programas.
extern GPIO_P<port2_traits>& port2;

}

#endif /* GPIO_HPP_ */
