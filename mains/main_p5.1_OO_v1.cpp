// edu@uma.es - 05/05/20 (v6)

// p5.1: Se implementar� el siguiente diagrama de estados (los eventos se generan al presionar(DOWN)/soltar(UP) Sx):
//	ST_ESTADO_A (INICIO) 	-> EV_S3_DOWN: ST_ESTADO_B (LED5+LED3)
//	ST_ESTADO_B (LED5+LED3) -> EV_S4_UP  : ST_ESTADO_C (LED6+LED4)
//								   -> EV_S6_UP  : ST_ESTADO_A (INICIO)
//	ST_ESTADO_C (LED6+LED4) -> EV_S5_DOWN: ST_ESTADO_B (LED5+LED3)
//	[Sx: Interrupci�n ANT 32ms WD ; SMCLK 1MHz CAL; LPM0]

#include <cstdint>
#include "watchdog.hpp"
#include "clock.hpp"
#include "digitalio.hpp"

constexpr const uint8_t EV_S3_DOWN = 0x01;
constexpr const uint8_t EV_S4_UP = 0x02;
constexpr const uint8_t EV_S5_DOWN = 0x04;
constexpr const uint8_t EV_S6_UP = 0x08;

uint8_t volatile eventos;

constexpr void SET(uint8_t ev) { eventos |= ev; }
constexpr void RESET(uint8_t ev) { eventos &= ~(ev); }
constexpr bool TEST(uint8_t ev)	{return (eventos & ev); }

using namespace euma;

void main(void) {

    constexpr const uint8_t ST_ESTADO_A = 0;
    constexpr const uint8_t ST_ESTADO_B = 1;
    constexpr const uint8_t ST_ESTADO_C = 2;

	uint8_t estado = ST_ESTADO_A;

	the_watchdog.stop();
	the_clock.set_dco(DCOFREQ::_1MHZ);

	port1.as_input(GPIO::BIT4, GPIO::PULLUP);
	port1.enable_interrupt(GPIO::BIT4, GPIO::FALLING);

    port2.as_digital_io(GPIO::BIT7+GPIO::BIT6);
	port2.as_input(GPIO::BIT3+GPIO::BIT2+GPIO::BIT1, GPIO::PULLUP);
	port2.enable_interrupt(GPIO::BIT3+GPIO::BIT2+GPIO::BIT1, GPIO::FALLING);
	port2.as_output(GPIO::BIT7+GPIO::BIT6+GPIO::BIT5+GPIO::BIT4);
	port2.reset_output(GPIO::BIT7+GPIO::BIT6+GPIO::BIT5+GPIO::BIT4);

	eventos = 0;

	while (1) {
		__low_power_mode_0();

		while (eventos) {

			switch (estado) {

				case (ST_ESTADO_A): {

					if (TEST(EV_S3_DOWN)) {
						RESET(EV_S3_DOWN);
						estado = ST_ESTADO_B;
						port2.set_output(GPIO::BIT6+GPIO::BIT4);
						port2.reset_output(GPIO::BIT6+GPIO::BIT4);
					}

					if (TEST(EV_S4_UP)) {
						RESET(EV_S4_UP);
					} // if (TEST(EV_S4_UP))

					if (TEST(EV_S5_DOWN)) {
						RESET(EV_S5_DOWN);
					} // if (TEST(EV_S5_DOWN))

					if (TEST(EV_S6_UP)) {
						RESET(EV_S6_UP);
					} // if (TEST(EV_S6_UP))

					break;
				} // case (ST_ESTADO_A)

				case (ST_ESTADO_B): {

					if (TEST(EV_S3_DOWN)) {
						RESET(EV_S3_DOWN);
					} // if (TEST(EV_S3_DOWN))

					if (TEST(EV_S4_UP)) {
						RESET(EV_S4_UP);
						estado = ST_ESTADO_C;
						port2.reset_output(GPIO::BIT6+GPIO::BIT4);
						port2.set_output(GPIO::BIT7+GPIO::BIT5);
					} // if (TEST(EV_S4_UP))

					if (TEST(EV_S5_DOWN)) {
						RESET(EV_S5_DOWN);
					} // if (TEST(EV_S5_DOWN))

					if (TEST(EV_S6_UP)) {
						RESET(EV_S6_UP);
						estado = ST_ESTADO_A;
						port2.reset_output(GPIO::BIT7+GPIO::BIT6+GPIO::BIT5+GPIO::BIT4);
					} // if (TEST(EV_S6_UP))

					break;
				} // case (ST_ESTADO_B)

				case (ST_ESTADO_C): {

					if (TEST(EV_S3_DOWN)) {
						RESET(EV_S3_DOWN);
					} // if (TEST(EV_S3_DOWN))

					if (TEST(EV_S4_UP)) {
						RESET(EV_S4_UP);
					} // if (TEST(EV_S4_UP))

					if (TEST(EV_S5_DOWN)) {
						RESET(EV_S5_DOWN);
						estado = ST_ESTADO_B;
						port2.set_output(GPIO::BIT6+GPIO::BIT4);
						port2.reset_output(GPIO::BIT7+GPIO::BIT5);
					} // if (TEST(EV_S5_DOWN))

					if (TEST(EV_S6_UP)) {
						RESET(EV_S6_UP);
					} // if (TEST(EV_S6_UP))

					break;
				} // case (ST_ESTADO_C)

			} // switch (estado)

		} // while (eventos)

	} // while (1)

} // void main(void)

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void) {

    if (port1.is_interrupt_pending(GPIO::BIT4)) {
        port1.disable_interrupt(GPIO::BIT4);
        the_watchdog.enable_timer_interrupt(WATCHDOG::_32ms);
        if (port1.is_interrupt_falling(GPIO::BIT4)) {
            SET(EV_S3_DOWN);
        }
        else {}
        port1.toggle_interrupt_edge(GPIO::BIT4);
    }
    if (eventos) {
        __low_power_mode_off_on_exit();
    }
} // __interrupt void RTI_P1(void)

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void) {

    if (port2.is_interrupt_pending(GPIO::BIT1)) {
        port2.disable_interrupt(GPIO::BIT1);
        the_watchdog.enable_timer_interrupt(WATCHDOG::_32ms);
        if (port2.is_interrupt_falling(GPIO::BIT1)) {}
        else {
            SET(EV_S4_UP);
        }
        port2.toggle_interrupt_edge(GPIO::BIT1);
    }
    if (port2.is_interrupt_pending(GPIO::BIT2)) {
          port2.disable_interrupt(GPIO::BIT2);
          the_watchdog.enable_timer_interrupt(WATCHDOG::_32ms);
          if (port2.is_interrupt_falling(GPIO::BIT2)) {
              SET(EV_S5_DOWN);
          }
          else {}
          port2.toggle_interrupt_edge(GPIO::BIT2);
      }
    if (port2.is_interrupt_pending(GPIO::BIT3)) {
          port2.disable_interrupt(GPIO::BIT3);
          the_watchdog.enable_timer_interrupt(WATCHDOG::_32ms);
          if (port2.is_interrupt_falling(GPIO::BIT3)) {}
          else {
              SET(EV_S6_UP);
          }
          port2.toggle_interrupt_edge(GPIO::BIT3);
      }

	if (eventos) {
		__low_power_mode_off_on_exit();
	} // if (eventos)

} // __interrupt void RTI_P2(void)

#pragma vector = WDT_VECTOR
__interrupt void RTI_WD(void) {
    the_watchdog.stop();
    the_watchdog.disable_timer_interrupt();
    port1.clear_interrupt_flags(GPIO::BIT4);
    port2.clear_interrupt_flags(GPIO::BIT3+GPIO::BIT2+GPIO::BIT1);
    port1.enable_interrupt(GPIO::BIT4);
    port2.enable_interrupt(GPIO::BIT3+GPIO::BIT2+GPIO::BIT1);
} // __interrupt void RTI_WD(void)
