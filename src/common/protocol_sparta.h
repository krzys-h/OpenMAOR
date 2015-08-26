#pragma once

#include "common/protocol_base.h"

// Wersja hardware, TODO: co się stanie jeśli zmienimy?
#define SPARTA_VERSION_HARDWARE 23
// Wersja firmware, TODO: co się stanie jeśli zmienimy?
#define SPARTA_VERSION_FIRMWARE 12
// Wartość oznaczająca "nie zmieniaj silników" w przychodzącej wiadomości SPARTA_STATUS
#define SPARTA_STATUS_NO_MOTOR_CHANGE 245

// Protokół SPARTA
// ===============
// K->R
// 0 - SPARTA_HEADER
// 1 - ID robota odbierającego (ROBOT_ID) lub BROADCAST_ID aby odebrały wszystkie
// 2 - komenda (SpartaCommand)
// 3 - argument
// K<-R
// 0 - SPARTA_HEADER
// 1 - ID robota wysyłającego (ROBOT_ID)
// 2 - komenda (SpartaCommand)
// 3 - argument

// Protokół SPARTA_STATUS
// ======================
// K->R
// 0 - SPARTA_HEADER_STATUS
// 1 - ID robota odbierającego (ROBOT_ID) lub BROADCAST_ID aby odebrały wszystkie
// 2 - nowe ustawienie lewego silnika, lub SPARTA_STATUS_NO_MOTOR_CHANGE
// 3 - nowe ustawienie prawego silnika, lub SPARTA_STATUS_NO_MOTOR_CHANGE
// K<-R
// 0 - SPARTA_HEADER_STATUS
// 1 - ID robota wysyłającego (SPARTA_ROBOTID)
// 2 - CMD_STAN_CZUJNIKI
// 3 - napięcie baterii
// 4 - status czujników linii // bit 0 = FL, 1 = FR, 2 = BL, 3 = BR
// 5 - status LEDów
// 6 - napięcie silnika lewego
// 7 - napięcie silnika prawego
// 8 - sonar lewy
// 9 - sonar prawy
// 10 - ADC0 / 4
// 11 - ADC1 / 4
// 12 - ADC2 / 4
// 13 - predkość silnika lewego
// 14 - predkość silnika prawego
// 15 - suma kontrolna (dwa najbardziej znaczące bity sumy bajtów 3-14)

typedef uint8_t SpartaRobotID;
#define BROADCAST_ID 0xFD

enum SpartaProtocolType {
    SPARTA_HEADER = 0xFE,
    SPARTA_HEADER_STATUS = 0xFC,
};

enum SpartaCommand {
    // Sterowanie chwytakiem
    CMD_RAMIE1_GORA = 20, // (nieużywane)
    CMD_RAMIE1_DOL = 21, // (nieużywane)
    CMD_RAMIE2_GORA = 22, // (nieużywane)
    CMD_RAMIE2_DOL = 23, // (nieużywane)
    CMD_CHWYTAK_OTWORZ = 24, // (nieużywane)
    CMD_CHWYTAK_ZAMKNIJ = 25, // (nieużywane)
    // Eeee.... czemu to jest dwa razy?
    CMD_ZAMKNIJ_CHWYTAK = 237, // (nieobsługiwane)
    CMD_OTWORZ_CHWYTAK = 238, // (nieobsługiwane)

    // Komendy wysylania programu
    CMD_ODBIERAJ_PROGRAM = 201, // (nieobsługiwane)
    CMD_CRC_H = 202, // (nieobsługiwane)
    CMD_CRC_L = 203, // (nieobsługiwane)
    CMD_ODBIERZ_DL_PROGRAMU_H = 204, // (nieobsługiwane)
    CMD_ODBIERZ_DL_PROGRAMU_L = 205, // (nieobsługiwane)
    CMD_ZWROC_STRUMIENIOWO_PROGRAM = 209, // (nieobsługiwane)
    CMD_ROZKAZ_USTAW_ADRES_H = 211, // (nieobsługiwane)
    CMD_ROZKAZ_USTAW_ADRES_L = 212, // (nieobsługiwane)
    CMD_ROZKAZ_ODBIERZ_BAJT = 213, // (nieobsługiwane)
    CMD_CRC_OK = 214, // (nieobsługiwane)
    CMD_CRC_ERROR = 215, // (nieobsługiwane)
    CMD_ERROR_MAX_PGM_SIZE_EXCEEDED = 217, // (nieobsługiwane)

    // Jakis NOP? Po co to?
    CMD_IGNORUJ_NASTEPNY_BAJT = 210, // (nieużywane)

    // Komendy ID
    CMD_ODBIERZ_NOWE_ID = 207, // K->R, odpowiedź: CMD_ZWRACAM_ID, (nieobsługiwane)
    CMD_ZWROC_SWOJE_ID = 208, // K->R, odpowiedź: CMD_ZWRACAM_ID
    CMD_ZWRACAM_ID = 216, // K<-R

    // Start/stop programu
    CMD_ROZKAZ_AKTYWACJA = 218, // K->R
    CMD_ROZKAZ_DEAKTYWACJA = 219, // K->R

    // Odczyt stanu robota
    CMD_STAN_BATERII = 220, // K<->R, odpowiedź tym samym
    CMD_STAN_CZ_LINII = 221, // K<->R, odpowiedź tym samym
    CMD_STAN_PRAD_L = 222, // K<->R, odpowiedź tym samym
    CMD_STAN_PRAD_R = 223, // K<->R, odpowiedź tym samym
    CMD_STAN_SONAR_L = 224, // K<->R, odpowiedź tym samym
    CMD_STAN_SONAR_R = 225, // K<->R, odpowiedź tym samym
    CMD_STAN_AKC_X = 226, // (nieużywane)
    CMD_STAN_AKC_Y = 227, // (nieużywane)
    CMD_STAN_AKC_Z = 228, // (nieużywane)
    CMD_STAN_CZUJNIKI = 229, // K<-R, PROTOKÓŁ SPARTA_STATUS, odpowiedź robota

    // Zdalne sterowanie
    CMD_SILNIK_LEWY = 231, // K->R
    CMD_SILNIK_PRAWY = 232, // K->R
    CMD_SILNIK_STOP = 233, // K->R
    CMD_LED = 234, // K->R

    // Informacje o wersji
    CMD_FIRMWARE_VER = 235, // K<->R, odpowiedź tym samym
    CMD_HARDWARE_VER = 236, // K<->R, odpowiedź tym samym
};

class CProtocolSparta : public CBufferedProtocol
{
public:
    CProtocolSparta(CUart* uart, CRemoteCommandExecutor* command) : CBufferedProtocol(uart, command) {};

    bool RecieveData(uint8_t byte);

protected:
    void ProcessPacket(SpartaProtocolType protocol, SpartaRobotID robotid, uint8_t data1, uint8_t data2);
    void ProcessPacketNormal(SpartaCommand cmd, uint8_t param);
    void ProcessPacketStatus(uint8_t param1, uint8_t param2);

    void SendPacketNormal(SpartaRobotID senderid, SpartaCommand cmd, uint8_t param);
    void SendPacketStatus(SpartaRobotID senderid, SpartaCommand cmd, const uint8_t (&params)[12]);
    void SendPacketNormal(SpartaCommand cmd, uint8_t param);
    void SendPacketStatus(SpartaCommand cmd, const uint8_t (&params)[12]);
};
