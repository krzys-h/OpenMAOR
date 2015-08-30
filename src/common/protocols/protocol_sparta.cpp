#include "common/protocols/protocol_sparta.h"

#include "common/framework.h"
#include "common/uart.h"

namespace OpenMAOR
{
namespace Protocols
{

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

void CProtocolSparta::ProcessPacket(SpartaProtocolType protocol, SpartaRobotID robotid, uint8_t data1, uint8_t data2)
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
void CProtocolSparta::ProcessPacketNormal(SpartaCommand cmd, uint8_t param)
{
    switch (cmd)
    {
        case CMD_ZWROC_SWOJE_ID:
        case CMD_ODBIERZ_NOWE_ID: // Nie obslugujemy ID
            SendPacketNormal(CMD_ZWRACAM_ID, BROADCAST_ID);
            break;

        case CMD_FIRMWARE_VER:
            SendPacketNormal(CMD_FIRMWARE_VER, SPARTA_VERSION_FIRMWARE);
            break;

        case CMD_HARDWARE_VER:
            SendPacketNormal(CMD_HARDWARE_VER, SPARTA_VERSION_HARDWARE);
            break;

        case CMD_SILNIK_LEWY:
            CFrameworkBase::motors.SetLeft(OpenMAOR::Peripherals::CMotors::PercentageToSpeed(param-100));
            break;

        case CMD_SILNIK_PRAWY:
            CFrameworkBase::motors.SetRight(OpenMAOR::Peripherals::CMotors::PercentageToSpeed(param-100));
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
            //TODO
            SendPacketNormal(CMD_STAN_BATERII, 0xFF);
            break;

        case CMD_STAN_CZ_LINII:
            CFrameworkBase::lineSensor.Enable(true);
            SendPacketNormal(CMD_STAN_CZ_LINII,
                (CFrameworkBase::lineSensor[0].Get() << 0) |
                (CFrameworkBase::lineSensor[1].Get() << 1) |
                (CFrameworkBase::lineSensor[2].Get() << 2) |
                (CFrameworkBase::lineSensor[3].Get() << 3)
            );
            CFrameworkBase::lineSensor.Enable(false);
            break;

        case CMD_STAN_PRAD_L:
            //TODO
            SendPacketNormal(CMD_STAN_PRAD_L, 0x00);
            break;

        case CMD_STAN_PRAD_R:
            //TODO
            SendPacketNormal(CMD_STAN_PRAD_R, 0x00);
            break;

        case CMD_STAN_SONAR_L:
            SendPacketNormal(CMD_STAN_SONAR_L, CFrameworkBase::sonar.Get().left);
            break;

        case CMD_STAN_SONAR_R:
            SendPacketNormal(CMD_STAN_SONAR_R, CFrameworkBase::sonar.Get().right);
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
            SendPacketNormal(CMD_ERROR_MAX_PGM_SIZE_EXCEEDED, 0x00);
            SendPacketNormal(CMD_CRC_ERROR, 0x00);
            break;

        case CMD_ROZKAZ_AKTYWACJA:
            //TODO
            break;

        case CMD_ROZKAZ_DEAKTYWACJA:
            //TODO
            break;
    }
}

void CProtocolSparta::ProcessPacketStatus(uint8_t motorLeft, uint8_t motorRight)
{
    if (motorLeft != SPARTA_STATUS_NO_MOTOR_CHANGE)
    {
        CFrameworkBase::motors.SetLeft(OpenMAOR::Peripherals::CMotors::PercentageToSpeed(motorLeft-100));
    }
    if (motorRight != SPARTA_STATUS_NO_MOTOR_CHANGE)
    {
        CFrameworkBase::motors.SetRight(OpenMAOR::Peripherals::CMotors::PercentageToSpeed(motorRight-100));
    }

    uint8_t data[12];
    data[0] = 0xFF;
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
    data[3] = 0x00;
    data[4] = 0x00;
    OpenMAOR::Peripherals::CSonar::SonarResult sonar = CFrameworkBase::sonar.Get();
    data[5] = sonar.left;
    data[6] = sonar.right;
    data[7] = (0x0000 >> 2);
    data[8] = (0x0000 >> 2);
    data[9] = (0x0000 >> 2);
    data[10] = OpenMAOR::Peripherals::CMotors::SpeedToPercentage(CFrameworkBase::motors.GetLeft())+100;
    data[11] = OpenMAOR::Peripherals::CMotors::SpeedToPercentage(CFrameworkBase::motors.GetRight())+100;

    SendPacketStatus(CMD_STAN_CZUJNIKI, data);
}

void CProtocolSparta::SendPacketNormal(SpartaRobotID senderid, SpartaCommand cmd, uint8_t param)
{
    m_uart->Send(SPARTA_HEADER);
    m_uart->Send(senderid);
    m_uart->Send(cmd);
    m_uart->Send(param);
}

void CProtocolSparta::SendPacketStatus(SpartaRobotID senderid, SpartaCommand cmd, const uint8_t (&params)[12])
{
    m_uart->Send(SPARTA_HEADER_STATUS);
    m_uart->Send(senderid);
    m_uart->Send(cmd);
    uint8_t checksum = 0;
    for(uint8_t i = 0; i < 12; i++)
    {
        checksum += params[i];
        m_uart->Send(params[i]);
    }
    m_uart->Send(checksum);
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
