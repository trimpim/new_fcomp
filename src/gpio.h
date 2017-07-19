#pragma once

#include "register.h"

#include <cstdint>


//
// (@ 0x500X0000) GPIO0 Structure
//
struct GpioCtrl {
  RegisterHelper<uint32_t> MASK;         // (@ 0x500X0000) Pin value mask register. Affects operations on PIN, OUT, SET, CLR, and NOT registers.
  RegisterHelper<uint32_t> PIN;          // (@ 0x500X0004) Pin value register.
  RegisterHelper<uint32_t> OUT;          // (@ 0x500X0008) Pin output value register.
  RegisterHelper<uint32_t> SET;          // (@ 0x500X000C) Pin output value set register.
  RegisterHelper<uint32_t> CLR;          // (@ 0x500X0010) Pin output value clear register.
  RegisterHelper<uint32_t> NOT;          // (@ 0x500X0014) Pin output value invert register.
  RegisterDummy<uint32_t> RES01;         // reserved do not use
  RegisterDummy<uint32_t> RES02;         // reserved do not use
  RegisterHelper<uint32_t> DIR;          // (@ 0x500X0020) Data direction register.
  RegisterHelper<uint32_t> IS;           // (@ 0x500X0024) Interrupt sense register.
  RegisterHelper<uint32_t> IBE;          // (@ 0x500X0028) Interrupt both edges register.
  RegisterHelper<uint32_t> IEV;          // (@ 0x500X002C) Interrupt event register.
  RegisterHelper<uint32_t> IE;           // (@ 0x500X0030) Interrupt mask register.
  RegisterHelper<uint32_t> RIS;          // (@ 0x500X0034) Raw interrupt status register.
  RegisterHelper<uint32_t> MIS;          // (@ 0x500X0038) Masked interrupt status register.
  RegisterHelper<uint32_t> IC;           // (@ 0x5000003C) Interrupt clear register.
};


//
//  ensure the size of the GpioCtrl structure is correct!
//
static_assert(sizeof(GpioCtrl) == 16*sizeof(uint32_t), "");

__attribute__((section(".gpio_port1")))
GpioCtrl gGpioPort1;

__attribute__((section(".gpio_port2")))
GpioCtrl gGpioPort2;
