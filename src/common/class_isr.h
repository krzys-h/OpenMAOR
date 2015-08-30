#pragma once
#include <avr/interrupt.h>

#define CLASS_ISR(classname, isr) ISR(isr) { classname::InterruptHandler_##isr(); } void classname::InterruptHandler_##isr()
#define DECLARE_CLASS_ISR(isr) static inline void InterruptHandler_##isr()
