#pragma once
#include "common/class_isr.h"
#include "common/extra_section.h"


namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

/**
 * \class CButton
 * \brief A button! :D
 */
class CButton
{
private:
    friend class OpenMAOR::CPeripherals;
    CButton() EXTRA;

public:
    static bool Get() EXTRA;

    typedef void(*ButtonCallback)();
    static void SetCallback(ButtonCallback callback) EXTRA;

    DECLARE_CLASS_ISR(INT1_vect);

private:
    static ButtonCallback m_callback;
};

} // namespace Peripherals
} // namespace OpenMAOR
