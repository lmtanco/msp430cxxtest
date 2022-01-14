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
#include "digitalio.hpp"

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

        enum pwmoutput {
            OUT0,
            OUT1,
            OUT2
        };

        template <typename Tag, typename T>
        struct Tagged
        {
          explicit Tagged(const T& value) : value{value} { }
          T value;
        };

        template <typename Tag, typename T>
        constexpr Tagged<Tag, T> tag(const T& value)
        {
          return Tagged<Tag, T>{value};
        }

        struct WorkCycleTag {};
        struct PeriodTag {};
        struct ToggleTimeTag {};
        struct SemiPeriodTag {};

        using WorkCycle = Tagged<WorkCycleTag, uint16_t>;
        using Period = Tagged<PeriodTag, uint16_t>;
        using ToggleTime = Tagged<ToggleTimeTag, uint16_t>;
        using Semiperiod = Tagged<SemiPeriodTag, uint16_t>;
    };

    // Particularidades del timer 1
    struct timer1_traits{
        // Dirección de comienzo de los registros del timer 1.
        static constexpr std::uintptr_t address{0x0180};
        static device_register16& _TAxIV; //TA1IV: 0x011E
        // TODO: static constexpr void select_out0_pin(){}
        // TODO: static constexpr void select_out1_pin(){}
        static constexpr void select_out2_pin(){
            _P2DIR |= GPIO::BIT5;
            _P2SEL |= GPIO::BIT5;
        }
    private:
        static device_register8& _P2SEL;  //P2SEL: 0x002E
        static device_register8& _P2SEL2; //P2SEL2:0x0042
        static device_register8& _P2DIR;  //P2DIR: 0x002A
    };

    // Particularidades del timer 0
    struct timer0_traits{
        // Dirección de comienzo de los registros del timer 0
        static constexpr std::uintptr_t address{0x0160};
        static device_register16& _TAxIV; //TA0IV: 0x012E
        // TODO: static constexpr void select_out0_pin(){}
        static constexpr void select_out1_pin(){
            _P1SEL |= GPIO::BIT6;
            _P1DIR |= GPIO::BIT6;
        }
        // TODO: static constexpr void select_out2_pin(){}
    private:
        static device_register8& _P1SEL;  //P1SEL: 0x0026
        static device_register8& _P1SEL2; //P1SEL2 0x0041
        static device_register8& _P1DIR;  //P1DIR: 0x0022
    };


    // Clase que empaqueta el timer A3 del msp4302553
    template <
              typename timer_traits
             >
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
                              TIMER::div clock_div)        {
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
        // Habilitar alguna de las interrupciones
        void disable_interrupt(TIMER::interr which) {
            constexpr uint16_t taifg = 0x0001;
            constexpr uint16_t taie  = 0x0002;
            constexpr uint16_t ccifg = 0x0001;
            constexpr uint16_t ccie  = 0x0010;
            switch (which) {
            case interr::TAIFG:
                _TAxCTL &= ~taie;
                _TAxCTL &= ~taifg;
                break;
            case interr::TACCR0_CCIFG:
                _TAxCCTL0 &= ~ccie;
                _TAxCCTL0 &= ~ccifg;
                break;
            case interr::TACCR1_CCIFG:
                _TAxCCTL1 &= ~ccie;
                _TAxCCTL1 &= ~ccifg;
                break;
            case interr::TACCR2_CCIFG:
                _TAxCCTL2 &= ~ccie;
                _TAxCCTL2 &= ~ccifg;
                break;
            }
        }

    protected:
        device_register16 _TAxCTL;
        device_register16 _TAxCCTL0;
        device_register16 _TAxCCTL1;
        device_register16 _TAxCCTL2;
        uint16_t          _padding[4]; // Los registros del temporizador A3 no están seguidos, hay un hueco.
        device_register16 _TAxR;
        device_register16 _TAxCCR0;
        device_register16 _TAxCCR1;
        device_register16 _TAxCCR2;
    };

    // Objetos globales para manejar el timer1
    extern TIMER_T<timer1_traits>& timer1;

    // Objetos globales para manejar el timer0
    extern TIMER_T<timer0_traits>& timer0;


}


#endif /* TIMER_HPP_ */
