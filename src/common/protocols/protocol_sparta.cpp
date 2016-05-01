#include "common/protocols/protocol_sparta.h"

#include "common/framework.h"
#include "common/uart.h"

using namespace OpenMAOR::Peripherals;

namespace OpenMAOR
{
namespace Protocols
{

namespace // anonymous namespace
{

//! Recieved packet buffer
static StdLib::CBuffer<uint8_t, 4> m_buffer;

void ProcessPacketNormal(SpartaCommand cmd, uint8_t param)
{
    switch (cmd)
    {
        case CMD_ZWROC_SWOJE_ID:
        case CMD_ODBIERZ_NOWE_ID: // Nie obslugujemy ID
            CProtocolSparta::SendPacketNormal(CMD_ZWRACAM_ID, BROADCAST_ID);
            break;

        case CMD_FIRMWARE_VER:
            CProtocolSparta::SendPacketNormal(CMD_FIRMWARE_VER, SPARTA_VERSION_FIRMWARE);
            break;

        case CMD_HARDWARE_VER:
            CProtocolSparta::SendPacketNormal(CMD_HARDWARE_VER, SPARTA_VERSION_HARDWARE);
            break;

        case CMD_SILNIK_LEWY:
            // TODO: CFrameworkBase::motors.SetLeft(::CMotors::PercentageToSpeed(param-100));
            break;

        case CMD_SILNIK_PRAWY:
            // TODO: CFrameworkBase::motors.SetRight(::CMotors::PercentageToSpeed(param-100));
            break;

        case CMD_SILNIK_STOP:
            CFrameworkBase::motors.Stop();
            break;

        case CMD_LED:
            CFrameworkBase::led[0].Set(param & (1<<0));
            CFrameworkBase::led[1].Set(param & (1<<1));
            CFrameworkBase::led[2].Set(param & (1<<2));
            CFrameworkBase::led[3].Set(param & (1<<3));
            break;

        case CMD_ZAMKNIJ_CHWYTAK:
        case CMD_OTWORZ_CHWYTAK:
            // Nie mamy chwytaka
            break;

        case CMD_STAN_BATERII:
            CProtocolSparta::SendPacketNormal(CMD_STAN_BATERII, CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_BATTERY)/10);
            break;

        case CMD_STAN_CZ_LINII:
            CFrameworkBase::lineSensor.Enable(true);
            CProtocolSparta::SendPacketNormal(CMD_STAN_CZ_LINII,
                (CFrameworkBase::lineSensor[0].Get() << 0) |
                (CFrameworkBase::lineSensor[1].Get() << 1) |
                (CFrameworkBase::lineSensor[2].Get() << 2) |
                (CFrameworkBase::lineSensor[3].Get() << 3)
            );
            CFrameworkBase::lineSensor.Enable(false);
            break;

        case CMD_STAN_PRAD_L:
            CProtocolSparta::SendPacketNormal(CMD_STAN_PRAD_L, CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_MOTOR_LEFT)*100/ADC_MOTOR_MAX_VALUE);
            break;

        case CMD_STAN_PRAD_R:
            CProtocolSparta::SendPacketNormal(CMD_STAN_PRAD_R, CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_MOTOR_RIGHT)*100/ADC_MOTOR_MAX_VALUE);
            break;

        case CMD_STAN_SONAR_L:
            CProtocolSparta::SendPacketNormal(CMD_STAN_SONAR_L, CFrameworkBase::sonar.Get().left);
            break;

        case CMD_STAN_SONAR_R:
            CProtocolSparta::SendPacketNormal(CMD_STAN_SONAR_R, CFrameworkBase::sonar.Get().right);
            break;

        // Nie obslugujemy wgrywania programu ze SPAR-TA (jeszcze)
        case CMD_ODBIERAJ_PROGRAM:
        case CMD_CRC_H:
        case CMD_CRC_L:
        case CMD_ODBIERZ_DL_PROGRAMU_H:
        case CMD_ODBIERZ_DL_PROGRAMU_L:
        case CMD_ZWROC_STRUMIENIOWO_PROGRAM:
        case CMD_ROZKAZ_USTAW_ADRES_H:
        case CMD_ROZKAZ_USTAW_ADRES_L:
        case CMD_ROZKAZ_ODBIERZ_BAJT:
            CProtocolSparta::SendPacketNormal(CMD_ERROR_MAX_PGM_SIZE_EXCEEDED, 0x00);
            CProtocolSparta::SendPacketNormal(CMD_CRC_ERROR, 0x00);
            break;

        case CMD_ROZKAZ_AKTYWACJA:
            //TODO
            break;

        case CMD_ROZKAZ_DEAKTYWACJA:
            //TODO
            break;
    }
}

void ProcessPacketStatus(uint8_t motorLeft, uint8_t motorRight)
{
    if (motorLeft != SPARTA_STATUS_NO_MOTOR_CHANGE)
    {
        CFrameworkBase::motors.SetLeft((((int16_t) motorLeft)-100) * 10); // not precise but meh
    }
    if (motorRight != SPARTA_STATUS_NO_MOTOR_CHANGE)
    {
        CFrameworkBase::motors.SetRight((((int16_t) motorRight)-100) * 10); // not precise but meh
    }

    uint8_t data[12];
    data[0] = CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_BATTERY) / 10;
    CFrameworkBase::lineSensor.Enable(true);
    data[1] =
        (CFrameworkBase::lineSensor[0].Get() << 0) |
        (CFrameworkBase::lineSensor[1].Get() << 1) |
        (CFrameworkBase::lineSensor[2].Get() << 2) |
        (CFrameworkBase::lineSensor[3].Get() << 3) ;
    CFrameworkBase::lineSensor.Enable(false);
    data[2] =
        (CFrameworkBase::led[0].Get() << 0) |
        (CFrameworkBase::led[1].Get() << 1) |
        (CFrameworkBase::led[2].Get() << 2) |
        (CFrameworkBase::led[3].Get() << 3) ;
    data[3] = CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_MOTOR_LEFT)*100/ADC_MOTOR_MAX_VALUE;
    data[4] = CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_MOTOR_RIGHT)*100/ADC_MOTOR_MAX_VALUE;
    ::CSonar::SonarResult sonar = CFrameworkBase::sonar.Get();
    data[5] = sonar.left;
    data[6] = sonar.right;
    data[7] = (CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_CUSTOM0) >> 2);
    data[8] = (CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_CUSTOM1) >> 2);
    data[9] = (CFrameworkBase::adc.Get(CADC::ADCChannel::ADC_CUSTOM2) >> 2);
    data[10] = CFrameworkBase::motors.GetLeft() / 10;
    data[11] = CFrameworkBase::motors.GetRight() / 10;

    CProtocolSparta::SendPacketStatus(CMD_STAN_CZUJNIKI, data);
}

void ProcessPacket(SpartaProtocolType protocol, SpartaRobotID robotid, uint8_t data1, uint8_t data2)
{
    // Sprawdzanie robotid pomijamy (na co nam to? :P)

    switch (protocol)
    {
        case SPARTA_HEADER:
            ProcessPacketNormal(static_cast<SpartaCommand>(data1), data2);
            break;

        case SPARTA_HEADER_STATUS:
            ProcessPacketStatus(data1, data2);
            break;
    }
}

} // anonymous namespace

bool CProtocolSparta::RecieveData(uint8_t data)
{
    if (m_buffer.Size() == 0)
    {
        if (data != SPARTA_HEADER && data != SPARTA_HEADER_STATUS)
            return false;
    }
    m_buffer.Add(data);
    if (m_buffer.Size() == 4)
    {
        ProcessPacket(static_cast<SpartaProtocolType>(m_buffer[0]), static_cast<SpartaRobotID>(m_buffer[1]), m_buffer[2], m_buffer[3]);
        m_buffer.Flush();
        return false;
    }
    return true;
}

void CProtocolSparta::SendPacketNormal(SpartaRobotID senderid, SpartaCommand cmd, uint8_t param)
{
    CUart::Send(SPARTA_HEADER);
    CUart::Send(senderid);
    CUart::Send(cmd);
    CUart::Send(param);
}

void CProtocolSparta::SendPacketStatus(SpartaRobotID senderid, SpartaCommand cmd, const uint8_t (&params)[12])
{
    CUart::Send(SPARTA_HEADER_STATUS);
    CUart::Send(senderid);
    CUart::Send(cmd);
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < 12; i++)
    {
        checksum += params[i];
        CUart::Send(params[i]);
    }
    CUart::Send(checksum);
}

void CProtocolSparta::SendPacketNormal(SpartaCommand cmd, uint8_t param)
{
    SendPacketNormal(BROADCAST_ID, cmd, param);
}

void CProtocolSparta::SendPacketStatus(SpartaCommand cmd, const uint8_t (&params)[12])
{
    SendPacketStatus(BROADCAST_ID, cmd, params);
}

} // namespace Protocols
} // namespace OpenMAOR
