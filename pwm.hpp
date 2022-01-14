/*
 * pwm.hpp
 *
 *  Created on: Jan 13, 2022
 *      Author: dianauma
 */

#ifndef PWM_HPP_
#define PWM_HPP_

#include "timer.hpp"
#include <type_traits>


namespace euma {

// Clase que define el interfaz de los objetos que valen para hacer pwm.
// Me gustaría no definirla y permitir sólo especializacioes pero el compilador no me deja
template < typename timer_traits,      // Qué temporizador: timer1_traits o timer0_traits
           TIMER::pwmoutput output > // Qué pwmoutput: OUT0, OUT1, ....
class PWM : public TIMER_T<timer_traits>
{
public:
    // Intento impedir que alguien cree objetos de esta clase.
    PWM() = delete;
};

// Clase que implementa la salida OUT0 (del bloque TAxCCR0).
//
template <typename timer_traits>
class PWM <timer_traits, TIMER::OUT0> : public TIMER_T<timer_traits> {
public:

    PWM() : TIMER_T<timer_traits>{}{}


    // Take pin: should this be inside the start_outmodeX?
    void take_pin() {
        timer_traits.::select_out0_pin();
    }

    // OUTMODE0: Ojo, borra entero TAxCCTL0
    void start_outmode0() {
       constexpr uint16_t out = 0x0004;
       constexpr uint16_t outmode0 = 0x0000 + out;
       _TAxCCTL0 = outmode0; // BORRAR outmode0?
    }
    void set_out() {
        constexpr uint16_t out = 0x0004;
        _TAxCCTL0 |= out;
    }
    void reset_out() {
       constexpr uint16_t out = 0x0004;
      _TAxCCTL0 &= ~out;
    }

    // OUTMODE1: Ojo, borra entero TAxCCTL0
    void start_outmode1() {
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode1 = 0x0020 + out;
        _TAxCCTL0 = outmode1;
    }

    // OUTMODE4: Ojo, borra entero TAxCCTL0
    void start_outmode4_up(uint16_t SemiPeriod) {  // TODO crear Tagged Type.
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode4 = 0x0080 + out;
        _TAxCCTL0 = outmode4;
        _TAxCCR0 = SemiPeriod;
        constexpr uint16_t taclr = 0x0004;
        _TAxCTL |= taclr | TIMER::UP;
    }

    // En modo continuo la salida es siempre de semiperiodo 65535.
    void start_outmode4_continuous(uint16_t ToggleMoment)  // TODO crear Tagged Type
    {
        constexpr uint16_t out = 0x0004;
         constexpr uint16_t outmode4 = 0x0080 + out;
         _TAxCCTL0 = outmode4;
         _TAxCCR0 = ToggleMoment;
         constexpr uint16_t taclr = 0x0004;
         _TAxCTL |= taclr | TIMER::CONTINUOUS;
    }

    // OUTMODE5: : Ojo, borra entero TAxCCTL0
    void start_outmode5(){
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode5 = 0x00A0 + out;
        _TAxCCTL0 |= outmode5; break;
    }

    // Parar el PWM saca 0v con outmode0 y pone out = 0.
    void stop_pwm() {
        // Debería llamar a stop(). TODO: implementar en clase TIMER_T método stop();
        start_outmode0();
        reset_out();
    }

    //static_assert(std::is_standard_layout<PWM<timer1_traits>>, "PWM<timer1_traits> is not standard layout")

};

// Clase que implementa la salida OUT1 (del bloque TAxCCR1).
//
template <typename timer_traits>
class PWM <timer_traits, TIMER::OUT1> : public TIMER_T<timer_traits> {
public:

    PWM() : TIMER_T<timer_traits>{}{}

    // Take pin: should this be inside each start_outmodeX?
    void take_pin() {
        timer_traits::select_out1_pin();
    }

    // TODO: void release_pin()

    // OUTMODE0: Ojo, borra entero TAxCCTL1
    void start_outmode0() {
       constexpr uint16_t out = 0x0004;
       constexpr uint16_t outmode0 = 0x0000 + out;
       _TAxCCTL1 = outmode0; // FIMXE: BORRAR outmode0?
    }
    void set_out() {
        constexpr uint16_t out = 0x0004;
        _TAxCCTL1 |= out;
    }
    void reset_out() {
       constexpr uint16_t out = 0x0004;
      _TAxCCTL1 &= ~out;
    }

    // OUTMODE1: Ojo, borra entero TAxCCTL1
    void start_outmode1() {
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode1 = 0x0020 + out;
        _TAxCCTL1 = outmode1;
    }

    // TODO: OUTMODE2, OUTMODE3

    // OUTMODE4: Ojo, borra entero TAxCCTL1
    void start_outmode4_up(uint16_t SemiPeriod, uint16_t ToggleMoment) {  // TODO crear Tagged Types.
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode4 = 0x0080 + out;
        _TAxCCTL1 = outmode4;
        _TAxCCR1 = ToggleMoment
        _TAxCCR0 = SemiPeriod;
        constexpr uint16_t taclr = 0x0004;
        _TAxCTL |= taclr | TIMER::UP;
    }

    // En modo continuo la salida es siempre de semiperiodo 65535.
    void start_outmode4_continuous(uint16_t ToggleMoment)  // TODO crear Tagged Types
    {
        constexpr uint16_t out = 0x0004;
         constexpr uint16_t outmode4 = 0x0080 + out;
         _TAxCCTL1 = outmode4;
         _TAxCCR1 = ToggleMoment;
         constexpr uint16_t taclr = 0x0004;
         _TAxCTL |= taclr | TIMER::CONTINUOUS;
    }

    // OUTMODE5
    void start_outmode5(){
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode5 = 0x00A0 + out;
        _TAxCCTL1 = outmode5;
    }

    // TODO: OUTMODE6

    // OUTMODE7
    void start_outmode7_up(uint16_t WorkCycle, uint16_t Period){
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode7 = 0x00E0 + out;
        _TAxCCTL1 = outmode7;
        _TAxCCR0 = Period;
        _TAxCCR1 = WorkCycle;
        constexpr uint16_t taclr = 0x0004;
        _TAxCTL |= taclr | TIMER::UP;

    }


    // Parar el PWM saca 0v con outmode0 y pone out = 0.
    void stop_pwm() {
        // Debería llamar a stop(). TODO: implementar en clase TIMER_T método stop();
        start_outmode0();
        reset_out();
    }
private:
    using TIMER_T<timer_traits>::_TAxCCR0;
    using TIMER_T<timer_traits>::_TAxCCR1;
    using TIMER_T<timer_traits>::_TAxCCTL1;
    using TIMER_T<timer_traits>::_TAxCTL;

    //static_assert(std::is_standard_layout<PWM<timer1_traits>>, "PWM<timer1_traits> is not standard layout")

};

// Clase que implementa la salida OUT2 (del bloque TAxCCR2).
//
template <typename timer_traits>
class PWM <timer_traits, TIMER::OUT2> : public TIMER_T<timer_traits>
{
public:

    PWM() : TIMER_T<timer_traits>{}{}


    // Take pin: should this be inside each start_outmodeX?
    void take_pin() {
        timer_traits::select_out2_pin();
    }

    // TODO: void release_pin()

    // OUTMODE0: Ojo, borra entero TAxCCTL2
    void start_outmode0() {
       constexpr uint16_t out = 0x0004;
       constexpr uint16_t outmode0 = 0x0000 + out;
       _TAxCCTL2 = outmode0; // FIMXE: BORRAR outmode0?
    }
    void set_out() {
        constexpr uint16_t out = 0x0004;
        _TAxCCTL2 |= out;
    }
    void reset_out() {
       constexpr uint16_t out = 0x0004;
      _TAxCCTL2 &= ~out;
    }

    // OUTMODE1: Ojo, borra entero TAxCCTL2
    void start_outmode1() {
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode1 = 0x0020 + out;
        _TAxCCTL2 = outmode1;
    }

    // TODO: OUTMODE2, OUTMODE3

    // OUTMODE4: Ojo, borra entero TAxCCTL2
    void start_outmode4_up(uint16_t SemiPeriod, uint16_t ToggleMoment) {  // TODO crear Tagged Types.
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode4 = 0x0080 + out;
        _TAxCCTL2 = outmode4;
        _TAxCCR2 = ToggleMoment
        _TAxCCR0 = SemiPeriod;
        constexpr uint16_t taclr = 0x0004;
        _TAxCTL |= taclr | TIMER::UP;
    }

    // En modo continuo la salida es siempre de semiperiodo 65535.
    void start_outmode4_continuous(uint16_t ToggleMoment)  // TODO crear Tagged Types
    {
        constexpr uint16_t out = 0x0004;
         constexpr uint16_t outmode4 = 0x0080 + out;
         _TAxCCTL2 = outmode4;
         _TAxCCR2 = ToggleMoment;
         constexpr uint16_t taclr = 0x0004;
         _TAxCTL |= taclr | TIMER::CONTINUOUS;
    }

    // OUTMODE5: : Ojo, borra entero TAxCCTL2
    void start_outmode5(){
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode5 = 0x00A0 + out;
        _TAxCCTL2 |= outmode5; break;
    }

    // TODO: OUTMODE6

    // OUTMODE7: Ojo, borra entero TAxCCTL2
    void start_outmode7_up(uint16_t WorkCycle, uint16_t Period){
        constexpr uint16_t out = 0x0004;
        constexpr uint16_t outmode7 = 0x00E0 + out;
        _TAxCCTL2 = outmode7;
        _TAxCCR0 = Period;
        _TAxCCR2 = WorkCycle;
        constexpr uint16_t taclr = 0x0004;
        _TAxCTL |= taclr | TIMER::UP;

    }


    // Parar el PWM saca 0v con outmode0 y pone out = 0.
    void stop_pwm() {
        // Debería llamar a stop(). TODO: implementar en clase TIMER_T método stop();
        start_outmode0();
        reset_out();
    }

private:
    using TIMER_T<timer_traits>::_TAxCCR0;
    using TIMER_T<timer_traits>::_TAxCCR2;
    using TIMER_T<timer_traits>::_TAxCTL;
    using TIMER_T<timer_traits>::_TAxCCTL2;


};


extern PWM<timer1_traits, TIMER::OUT0>& pwm_timer1_out0;
extern PWM<timer1_traits, TIMER::OUT1>& pwm_timer1_out1;
extern PWM<timer1_traits, TIMER::OUT2>& pwm_timer1_out2;

//using testtype = PWM<timer1_traits, TIMER::OUT2>;
//static_assert(std::is_standard_layout<testtype>::value, "PWM<timer1_traits, TIMER::OUT2> is not standard layout");


extern PWM<timer0_traits, TIMER::OUT0>& pwm_timer0_out0;
extern PWM<timer0_traits, TIMER::OUT1>& pwm_timer0_out1;
extern PWM<timer0_traits, TIMER::OUT2>& pwm_timer0_out2;

}

#endif /* PWM_HPP_ */
