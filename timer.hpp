/*
 * timer.hpp
 *
 *  Created on: Jan 8, 2022
 *      Author: Luis (lmtanco@uma.es)
 *
 *      Following the teachings of Dan & Ben Saks
 */

#ifndef EUMA_TIMER_HPP_
#define EUMA_TIMER_HPP_

#include "common.hpp"

namespace euma {

    // Clase para definir tipos enumerados.
    class TIMER {
    public:
        enum clk : uint16_t {
            ACLK =  0x0100,
            SMCLK = 0x0200
        };
        enum mode : uint16_t {
            STOP =       0x0000,
            UP =         0x0010,
            CONTINUOUS = 0x0020,
            UPDOWN =     0x0030
        };
        enum div : uint16_t {
            _1 = 0x0000,
            _2 = 0x0040,
            _4 = 0x0080,
            _8 = 0x00C0
        };

        enum interr : uint16_t {
            TAIFG,
            TACCR0_CCIFG,
            TACCR1_CCIFG,
            TACCR2_CCIFG
        };
    };

    // Particularidades del timer 1
    struct timer1_A3_traits{
        static constexpr std::uintptr_t address{0x0180};
        static device_register16& _TAxIV; //0x011E;
    };

    // Particularidades del timer 0
    struct timer0_A3_traits{
        static constexpr std::uintptr_t address{0x0160};
        static device_register16& _TAxIV; //0x012E;
    };


    // Clase que empaqueta el timer A3 del msp4302553
    template <typename timer_traits>
    class TIMER_T : TIMER {
    public:

        // Redefinimos operador new para mapear el objeto en memoria.
        void *operator new(std::size_t) {
            return reinterpret_cast<void*>(timer_traits::address);
        }

        // Delete no tiene que hacer nada.
        void operator delete(void * ) {}

        // Imprescindible definir el constructor para que el compilador no llame a
        // memset con 0s después de new.
        TIMER_T()
        {
            constexpr uint16_t taclr = 0x0004;
            constexpr uint16_t clock_source = TIMER::SMCLK;
            constexpr uint16_t clock_div = TIMER::_8;
            _TAxCTL = taclr | clock_source | clock_div;
        }

        // Enganchar a la fuente de reloj
        void set_clock_source(TIMER::clk clock_source,
                              TIMER::div clock_div)
        {
            _TAxCTL |= clock_source | clock_div;
        }

        // Si em modo es TIMER::CONTINUOUS, MAX_TACCR0 se ignora
        void start(TIMER::mode timer_mode, uint16_t MAX_TACCR0 )
        {
            constexpr uint16_t taclr = 0x0004;
            _TAxCTL |= taclr | timer_mode;
            if (timer_mode == TIMER::UP) {
                _TAxCCR0 = MAX_TACCR0;
            }
        }

        // Habilitar alguna de las interrupciones
        void enable_interrupt(TIMER::interr which) {
            constexpr uint16_t taifg = 0x0001;
            constexpr uint16_t taie  = 0x0002;
            constexpr uint16_t ccifg = 0x0001;
            constexpr uint16_t ccie  = 0x0010;
            switch (which) {
            case interr::TAIFG:
                _TAxCTL |= taie;
                _TAxCTL &= ~taifg;
                break;
            case interr::TACCR0_CCIFG:
                _TAxCCTL0 |= ccie;
                _TAxCCTL0 &= ~ccifg;
                break;
            case interr::TACCR1_CCIFG:
                _TAxCCTL1 |= ccie;
                _TAxCCTL1 &= ~ccifg;
                break;
            case interr::TACCR2_CCIFG:
                _TAxCCTL2 |= ccie;
                _TAxCCTL2 &= ~ccifg;
                break;
            }
        }


    private:
        device_register16 _TAxCTL;
        device_register16 _TAxCCTL0;
        device_register16 _TAxCCTL1;
        device_register16 _TAxCCTL2;
        uint16_t          _padding[4];
        device_register16 _TAxR;
        device_register16 _TAxCCR0;
        device_register16 _TAxCCR1;
        device_register16 _TAxCCR2;
    };

    // Objetos globales para manejar el timer1
    extern TIMER_T<timer1_A3_traits>& timer1;

    // Objetos globales para manejar el timer0
    extern TIMER_T<timer0_A3_traits>& timer0;



}


#endif /* TIMER_HPP_ */
