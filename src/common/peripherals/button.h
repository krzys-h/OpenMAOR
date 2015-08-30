#pragma once
#include "common/singleton.h"
#include "common/class_isr.h"

#include "common/extra_section.h"


namespace OpenMAOR
{
class CPeripherals;

namespace Peripherals
{

// TODO: CSingleton jest tu tylko dla ISR, nie da sie tego zrobic inaczej?

/**
 * \class CButton
 * \brief A button! :D
 */
class CButton : public CSingleton<CButton>
{
private:
    friend class OpenMAOR::CPeripherals;
    CButton() EXTRA;

public:
    bool Get() EXTRA;

    typedef void(*ButtonCallback)();
    void SetCallback(ButtonCallback callback) EXTRA;

    DECLARE_CLASS_ISR(INT1_vect);

private:
    ButtonCallback m_callback;
};

} // namespace Peripherals
} // namespace OpenMAOR
