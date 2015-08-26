#include "common/stdlib/assert.h"

extern "C" void __cxa_pure_virtual()
{
    assert_failed("Tried to call pure virtual function!");
}
