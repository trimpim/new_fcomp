#include "gpio.h"

#include <cstdint>
#include <array>
#include <limits>


//void display1() {
//
//  static int p = 0;
//
//  if (p == 0) {
//    gGpio2()->CLR |= (1 << 5);   // clear red LED
//    gGpio2()->SET |= (1 << 4);   // set green LED
//    p = 1;
//  } else {
//    gGpio2()->CLR |= (1 << 4);   // clear green LED
//    gGpio2()->SET |= (1 << 5);   // set red LED
//    p = 0;
//  }
//}


void display2() {
//
//  static int p = 0;
//
//  if (p == 0) {
//    gGpios.Port2.SET |= (1 << 5);   // clear red LED
////    gGpios.p2()->SET |= (1 << 4);   // set green LED
//    p = 1;
//  } else {
////    gGpios.p2()->CLR |= (1 << 4);   // clear green LED
////    gGpios.p2()->CLR |= (1 << 5);   // set red LED
//    p = 0;
//  }
}


__attribute__((noinline))
void setup() {

  // set pin directions for LED pins
//  gGpioPort2.DIR = (1 << 4);
  gGpioPort2.DIR |= (1 << 4);
//  gGpioPort2.DIR.set((1 << 5), (1 << 4));

//  volatile auto c = gGpioPort2.DIR();
//  gGpioPort2.DIR = c;

  // clear all LEDs
//  gGpios.p2()->CLR |= (1 << 4);
//  gGpios.p2()->CLR |= (1 << 5);
}


/**
 *
 *  The interrupt service routine called when the processor starts or comes out
 *  of a reset.
 *  This method should never rerurn!
 *
 **/
__attribute__((section(".isr_functions"), isr))
void ResetISR() {

  volatile uint32_t a = 0;

  setup();
  display2();

  while (1)
  {
    ++a;
    if (a > 1000000) {
      display2();
      a = 0;
    }
  }
}

__attribute__((section(".isr_functions"), isr))
void NmiISR() {
  asm("bkpt;");
}


//
// Type for interrupt functions
//
//using IsrFunction = std::function<void()>;
typedef void (*IsrFunction)();


//
// List of all interrupts
//
enum class Interrupts {
  ResetISR = 0,
  NMI_Handler,
  HardFault_Handler,
  reserve01,
  reserve02,
  reserve03,
  reserve04,
  reserve05,
  reserve06,
  reserve07,
  SvcHandler,
  reserve08,
  reserve09,
  PendSvHandler,
  SysTickHandler,
  // External Interrupts
  WAKEUP_IRQHandler01,
  WAKEUP_IRQHandler02,
  WAKEUP_IRQHandler03,
  WAKEUP_IRQHandler04,
  WAKEUP_IRQHandler05,
  WAKEUP_IRQHandler06,
  WAKEUP_IRQHandler07,
  WAKEUP_IRQHandler08,
  WAKEUP_IRQHandler09,
  WAKEUP_IRQHandler0A,
  WAKEUP_IRQHandler0B,
  WAKEUP_IRQHandler0C,
  // internal IP core Interrupts
  I2C_IRQHandler,
  TIMER16_0_IRQHandler,
  TIMER16_1_IRQHandler,
  TIMER32_0_IRQHandler,
  TIMER32_1_IRQHandler,
  SSP_IRQHandler,
  UART0_IRQHandler,
  UART1_IRQHandler,
  COMP_IRQHandler,
  ADC_IRQHandler,
  WDT_IRQHandler,
  BOD_IRQHandler,
  reserve0A,
  PIOINT0_IRQHandler,
  PIOINT1_IRQHandler,
  PIOINT2_IRQHandler,
  reserve0B,
  DMA_IRQHandler,
  RTC_IRQHandler,
  InterruptLast
};


//
// template alias for interrupt handler creation
//
template<typename T>
using InterruptHandler = std::pair<Interrupts, T>;


//
// external (linker) defined symbol for the top of the stack
//
extern uint32_t _vStackTop;


//
// Class that models the processor
//
template<typename...VL>
struct NXP_LPC_1227 {

  uint32_t* mStackTop;
  std::array<IsrFunction, static_cast<int>(Interrupts::InterruptLast)> mIsrVectors;
  std::array<Interrupts, static_cast<int>(Interrupts::InterruptLast)> mIndexes;

  constexpr NXP_LPC_1227(InterruptHandler<VL> ... interruptHandlers) :
    mStackTop{&_vStackTop}, mIsrVectors{interruptHandlers.second...}, mIndexes{interruptHandlers.first...}
  { }

  constexpr std::size_t get_index(Interrupts irq) {
    for (std::size_t idx=0; mIndexes.size() >= idx; ++idx) {
      if (mIndexes[idx] == irq) {
        return idx;
      }
    }
    return std::numeric_limits<std::size_t>::max();
  }
};


//
// helper to create an instance of the processor class
//
template<typename ... T>
constexpr auto make_NXP_LPC_1227(InterruptHandler<T> ... t) {
  return NXP_LPC_1227<T...>{std::move(t)...};
}


//
// helper to attach interrupt handlers
//
template<typename T>
constexpr auto make_interrup_handler(Interrupts i, T t) {
  return InterruptHandler<T>{i, std::move(t)};
}



//
// global instance of the processor class
//
__attribute__((section(".isr_vector_list")))
auto gVectorTable = make_NXP_LPC_1227(
  make_interrup_handler(Interrupts::ResetISR,               ResetISR),
  make_interrup_handler(Interrupts::NMI_Handler,            NmiISR),
  make_interrup_handler(Interrupts::HardFault_Handler,      nullptr),
  make_interrup_handler(Interrupts::reserve01,              nullptr),
  make_interrup_handler(Interrupts::reserve02,              nullptr),
  make_interrup_handler(Interrupts::reserve03,              nullptr),
  make_interrup_handler(Interrupts::reserve04,              nullptr),
  make_interrup_handler(Interrupts::reserve05,              nullptr),
  make_interrup_handler(Interrupts::reserve06,              nullptr),
  make_interrup_handler(Interrupts::reserve07,              nullptr),
  make_interrup_handler(Interrupts::SvcHandler,             nullptr),
  make_interrup_handler(Interrupts::reserve08,              nullptr),
  make_interrup_handler(Interrupts::reserve09,              nullptr),
  make_interrup_handler(Interrupts::PendSvHandler,          nullptr),
  make_interrup_handler(Interrupts::SysTickHandler,         nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler01,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler02,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler03,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler04,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler05,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler06,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler07,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler08,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler09,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler0A,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler0B,    nullptr),
  make_interrup_handler(Interrupts::WAKEUP_IRQHandler0C,    nullptr),
  make_interrup_handler(Interrupts::I2C_IRQHandler,         nullptr),
  make_interrup_handler(Interrupts::TIMER16_0_IRQHandler,   nullptr),
  make_interrup_handler(Interrupts::TIMER16_1_IRQHandler,   nullptr),
  make_interrup_handler(Interrupts::TIMER32_0_IRQHandler,   nullptr),
  make_interrup_handler(Interrupts::TIMER32_1_IRQHandler,   nullptr),
  make_interrup_handler(Interrupts::SSP_IRQHandler,         nullptr),
  make_interrup_handler(Interrupts::UART0_IRQHandler,       nullptr),
  make_interrup_handler(Interrupts::UART1_IRQHandler,       nullptr),
  make_interrup_handler(Interrupts::COMP_IRQHandler,        nullptr),
  make_interrup_handler(Interrupts::ADC_IRQHandler,         nullptr),
  make_interrup_handler(Interrupts::WDT_IRQHandler,         nullptr),
  make_interrup_handler(Interrupts::BOD_IRQHandler,         nullptr),
  make_interrup_handler(Interrupts::reserve0A,              nullptr),
  make_interrup_handler(Interrupts::PIOINT0_IRQHandler,     nullptr),
  make_interrup_handler(Interrupts::PIOINT1_IRQHandler,     nullptr),
  make_interrup_handler(Interrupts::PIOINT2_IRQHandler,     nullptr),
  make_interrup_handler(Interrupts::reserve0B,              nullptr),
  make_interrup_handler(Interrupts::DMA_IRQHandler,         nullptr),
  make_interrup_handler(Interrupts::RTC_IRQHandler,         nullptr)
);

