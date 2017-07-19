#pragma once

#include <cstdint>


//
//  TODO ::: add second type that is used to ensure, only correct values are set (if possible)!
//
template<typename T>
struct RegisterHelper {

  volatile T mRegister;

  constexpr RegisterHelper() = default;

  constexpr void operator= (uint32_t rhs) { mRegister = rhs; }

//  constexpr uint32_t operator() () const {return mRegister; }      // TODO:: check if this is correct/desired behavior
//
//  template<typename ... RHS>
//  constexpr volatile T operator|= (const RHS ... rhs) { return (mRegister ... |= rhs); }
//
//  template<typename ... RHS>
//  constexpr volatile T operator&= (const RHS ... rhs) { return (mRegister ... &= rhs); }
//
//  template<typename ... RHS>
//  constexpr void set(RHS ... rhs) { (mRegister ... |= rhs); }
//
//  template<typename ... RHS>
//  constexpr void clear(RHS ... rhs) { (mRegister ... &= rhs); }

//  constexpr volatile T& operator|= (const uint32_t& rhs) { mRegister |= rhs; return *this; }
};

template<typename T>
constexpr volatile T& operator|= (T& lhs, const uint32_t rhs) { lhs.mRegister |= rhs; return lhs; }


template<typename T>
struct RegisterDummy {

  T mRegister;

  constexpr RegisterDummy() = default;
};
