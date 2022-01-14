/*
 * pwm.cpp
 *
 *  Created on: Jan 13, 2022
 *      Author: Luis (lmtanco@uma.es)
 *
 */

#include "pwm.hpp"

namespace euma {
PWM<timer1_traits, TIMER::OUT0>& pwm_timer1_out0 = *new PWM<timer1_traits, TIMER::OUT0>;
PWM<timer1_traits, TIMER::OUT1>& pwm_timer1_out1 = *new PWM<timer1_traits, TIMER::OUT1>;
PWM<timer1_traits, TIMER::OUT2>& pwm_timer1_out2 = *new PWM<timer1_traits, TIMER::OUT2>;


PWM<timer0_traits, TIMER::OUT0>& pwm_timer0_out0 = *new PWM<timer0_traits, TIMER::OUT0>;
PWM<timer0_traits, TIMER::OUT1>& pwm_timer0_out1 = *new PWM<timer0_traits, TIMER::OUT1>;
PWM<timer0_traits, TIMER::OUT2>& pwm_timer0_out2 = *new PWM<timer0_traits, TIMER::OUT2>;
}


