#include "common/framework.h"

namespace OpenMAOR
{

template<> CFrameworkBase* CSingleton<CFrameworkBase>::m_instance = nullptr;

void CFrameworkBase::UartCallback(uint8_t data)
{
    GetInstance().RecieveData(data);
}

} // namespace OpenMAOR
