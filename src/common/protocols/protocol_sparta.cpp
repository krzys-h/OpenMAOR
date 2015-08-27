#include "common/protocols/protocol_sparta.h"

#include "common/remote_command.h"
#include "common/uart.h"

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
            m_command->SetLeftEngine(((static_cast<float>(param)-100) / 100));
            break;

        case CMD_SILNIK_PRAWY:
            m_command->SetRightEngine(((static_cast<float>(param)-100) / 100));
            break;

        case CMD_SILNIK_STOP:
            m_command->SetLeftEngine(0.0f);
            m_command->SetRightEngine(0.0f);
            break;

        case CMD_LED:
            m_command->SetLED(0, param & (1<<0));
            m_command->SetLED(1, param & (1<<1));
            m_command->SetLED(2, param & (1<<2));
            m_command->SetLED(3, param & (1<<3));
            break;

        case CMD_ZAMKNIJ_CHWYTAK:
        case CMD_OTWORZ_CHWYTAK:
            // Nie mamy chwytaka
            break;

        case CMD_STAN_BATERII:
            SendPacketNormal(CMD_STAN_BATERII, m_command->GetBatteryStatus());
            break;

        case CMD_STAN_CZ_LINII:
            SendPacketNormal(CMD_STAN_CZ_LINII,
                (m_command->GetLineSensor(0) << 0) |
                (m_command->GetLineSensor(1) << 1) |
                (m_command->GetLineSensor(2) << 2) |
                (m_command->GetLineSensor(3) << 3)
            );
            break;

        case CMD_STAN_PRAD_L:
            SendPacketNormal(CMD_STAN_PRAD_L, m_command->GetLeftEnginePower());
            break;

        case CMD_STAN_PRAD_R:
            SendPacketNormal(CMD_STAN_PRAD_R, m_command->GetRightEnginePower());
            break;

        case CMD_STAN_SONAR_L:
            SendPacketNormal(CMD_STAN_SONAR_L, m_command->GetLeftSonar());
            break;

        case CMD_STAN_SONAR_R:
            SendPacketNormal(CMD_STAN_SONAR_R, m_command->GetRightSonar());
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
            m_command->StartProgram();
            break;

        case CMD_ROZKAZ_DEAKTYWACJA:
            m_command->StopProgram();
            break;
    }
}

void CProtocolSparta::ProcessPacketStatus(uint8_t motorLeft, uint8_t motorRight)
{
    if (motorLeft != SPARTA_STATUS_NO_MOTOR_CHANGE)
    {
        m_command->SetLeftEngine((static_cast<float>(motorLeft)-100)/100);
    }
    if (motorRight != SPARTA_STATUS_NO_MOTOR_CHANGE)
    {
        m_command->SetRightEngine((static_cast<float>(motorLeft)-100)/100);
    }

    uint8_t data[12];
    data[0] = m_command->GetBatteryStatus();
    data[1] =
        (m_command->GetLineSensor(0) << 0) |
        (m_command->GetLineSensor(1) << 1) |
        (m_command->GetLineSensor(2) << 2) |
        (m_command->GetLineSensor(3) << 3) ;
    data[2] =
        (m_command->GetLED(0) << 0) |
        (m_command->GetLED(1) << 1) |
        (m_command->GetLED(2) << 2) |
        (m_command->GetLED(3) << 3) ;
    data[3] = m_command->GetLeftEnginePower();
    data[4] = m_command->GetRightEnginePower();
    data[5] = m_command->GetLeftSonar();
    data[6] = m_command->GetRightSonar();
    data[7] = static_cast<uint8_t>(m_command->GetCustomADC(0) >> 2);
    data[8] = static_cast<uint8_t>(m_command->GetCustomADC(1) >> 2);
    data[9] = static_cast<uint8_t>(m_command->GetCustomADC(2) >> 2);
    data[10] = static_cast<uint8_t>(m_command->GetLeftEngine()+100);
    data[11] = static_cast<uint8_t>(m_command->GetRightEngine()+100);

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
