/*
 * clock.hpp
 *
 *  Created on: Jan 1, 2022
 *      Author: dianauma
 */

#ifndef EUMA_CLOCK_HPP_
#define EUMA_CLOCK_HPP_

#include "common.hpp"
#include <array>

namespace euma {

using memory_byte = uint8_t volatile;


enum class DCOFREQ {
    _16MHZ,
    _12MHZ,
    _8MHZ,
    _1MHZ
};

class DCOCALIBRATION {
public:
     // una forma de hacer que el watchdog esté bien alineado
     // en memoria es redefinir el operador new.
     static void* operator new(std::size_t) {
         return reinterpret_cast<void*>(0x10F8);
     }

     // Devolver el valor de la calibración para el registro DCO.
     memory_byte get_dco_calibration(DCOFREQ which) {
         memory_byte temp;
         switch (which) {
         case DCOFREQ::_16MHZ:
             temp = CALDCO_16MHZ;
             break;
         case DCOFREQ::_12MHZ:
             temp = CALDCO_12MHZ;
             break;
         case DCOFREQ::_8MHZ:
             temp = CALDCO_8MHZ;
             break;
         case DCOFREQ::_1MHZ:
             temp = CALDCO_1MHZ;
             break;
         }
         return temp;
     }
     // Devolver el valor de la calibración para el registro DCO.
     memory_byte get_bcs1_calibration(DCOFREQ which) {
         memory_byte temp;
         switch (which) {
         case DCOFREQ::_16MHZ:
             temp = CALBC1_16MHZ;
         case DCOFREQ::_12MHZ:
             temp = CALBC1_12MHZ;
         case DCOFREQ::_8MHZ:
             temp = CALBC1_8MHZ;
         case DCOFREQ::_1MHZ:
             temp = CALBC1_1MHZ;
         }
         return temp;
     }

private:
     memory_byte CALDCO_16MHZ;
     memory_byte CALBC1_16MHZ;
     memory_byte CALDCO_12MHZ;
     memory_byte CALBC1_12MHZ;
     memory_byte CALDCO_8MHZ;
     memory_byte CALBC1_8MHZ;
     memory_byte CALDCO_1MHZ;
     memory_byte CALBC1_1MHZ;
};

// Nos aseguramos de que el layout de la clase es el esperado:
   // - El primer atributo no estático está a offset cero
   // - Cada atributo tiene un offset mayor que el atributo justo antes que él.
   static_assert(
           std::is_standard_layout<DCOCALIBRATION>::value,
           "CLOCK isn't standard layout"
           );

   // La comprobación anterior es necesaria pero no suficiente.
   // Esto es especialmente necesario si hay más de un registro
   // Sirve para comprobar que el compilador no ha metido relleno entre medias
   // del los atributos.
   static_assert(
           sizeof(DCOCALIBRATION) == 8 * sizeof(memory_byte),
           "CLOCK contains extra padding bytes"
           );


DCOCALIBRATION& cal=*new DCOCALIBRATION{};

    // Clase que empaqueta el módulo de reloj del msp430g25353
    class CLOCK {
    public:

        void set_dco(DCOFREQ which) {
            _DCOCTL = 0;
            _BCSCTL1 = cal.get_bcs1_calibration(which);
            _DCOCTL = cal.get_dco_calibration(which); // cheating! Tengo q crear clase para esto.
        }

        static void* operator new(std::size_t) {
            return reinterpret_cast<void*>(0x8056);
        }
    private:
        device_register8 _DCOCTL;
        device_register8 _BCSCTL1;
        device_register8 _BCSCTL2;
        device_register8 _BCSCTL3;
    };
    // Nos aseguramos de que el layout de la clase es el esperado:
    // - El primer atributo no estático está a offset cero
    // - Cada atributo tiene un offset mayor que el atributo justo antes que él.
    static_assert(
            std::is_standard_layout<CLOCK>::value,
            "CLOCK isn't standard layout"
            );

    // La comprobación anterior es necesaria pero no suficiente.
    // Esto es especialmente necesario si hay más de un registro
    // Sirve para comprobar que el compilador no ha metido relleno entre medias
    // del los atributos.
    static_assert(
            sizeof(CLOCK) == 4 * sizeof(device_register8),
            "CLOCK contains extra padding bytes"
            );

    CLOCK& the_clock=*new CLOCK{};

}




#endif /* CLOCK_HPP_ */
