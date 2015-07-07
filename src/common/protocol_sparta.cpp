#include <avr/io.h>
#include "common/protocol_sparta.h"
#include "platform.h"
#include "common/uart.h"

void sparta_process_message(SpartaCommand command, uint8_t param);
void sparta_process_message_status(uint8_t motor_left, uint8_t motor_right);
void sparta_send(SpartaCommand command, uint8_t param);
void sparta_send_status(SpartaCommand command, uint8_t data[12]);

void sparta_byte_recieved(uint8_t byte)
{
    uint8_t sparta_msg_type = byte;
    uint8_t sparta_robot_id = uart_read();
    uint8_t sparta_command = uart_read();
    uint8_t sparta_param = uart_read();

    if(sparta_robot_id != ROBOT_ID && sparta_robot_id != BROADCAST_ID)
        return;

    if(sparta_msg_type == SPARTA_HEADER) {
        sparta_process_message((SpartaCommand)sparta_command, sparta_param);
    } else if(sparta_msg_type == SPARTA_HEADER_STATUS) {
        sparta_process_message_status(sparta_command, sparta_param);
    }
}

void sparta_process_message(SpartaCommand command, uint8_t param)
{
    switch(command) {
        case CMD_ZWROC_SWOJE_ID:
        case CMD_ODBIERZ_NOWE_ID: // Nie obslugujemy zmiany ID, mówimy komputerowi że zmienił na to samo
            sparta_send(CMD_ZWRACAM_ID, ROBOT_ID);
            break;

        case CMD_FIRMWARE_VER:
            sparta_send(CMD_FIRMWARE_VER, SPARTA_VERSION_FIRMWARE);
            break;

        case CMD_HARDWARE_VER:
            sparta_send(CMD_HARDWARE_VER, SPARTA_VERSION_HARDWARE);
            break;

        case CMD_SILNIK_LEWY:
            //TODO
            break;

        case CMD_SILNIK_PRAWY:
            //TODO
            break;

        case CMD_SILNIK_STOP:
            //TODO
            break;

        case CMD_LED:
            //TODO
            break;

        case CMD_ZAMKNIJ_CHWYTAK:
        case CMD_OTWORZ_CHWYTAK:
            // Nie mamy chwytaka
            break;

        case CMD_STAN_BATERII:
            //TODO
            sparta_send(CMD_STAN_BATERII, 0xFF);
            break;

        case CMD_STAN_CZ_LINII:
            //TODO
            sparta_send(CMD_STAN_CZ_LINII, 0x00);
            break;

        case CMD_STAN_PRAD_L:
            //TODO
            sparta_send(CMD_STAN_PRAD_L, 0xFF);
            break;

        case CMD_STAN_PRAD_R:
            //TODO
            sparta_send(CMD_STAN_PRAD_R, 0xFF);
            break;

        case CMD_STAN_SONAR_L:
            //TODO
            sparta_send(CMD_STAN_SONAR_L, 0x00);
            break;

        case CMD_STAN_SONAR_R:
            //TODO
            sparta_send(CMD_STAN_SONAR_R, 0x00);
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
            sparta_send(CMD_ERROR_MAX_PGM_SIZE_EXCEEDED, 0x00);
            sparta_send(CMD_CRC_ERROR, 0x00);
            break;

        case CMD_ROZKAZ_AKTYWACJA:
            //TODO
            //exit_bootloader = true;
            break;

        case CMD_ROZKAZ_DEAKTYWACJA:
            //TODO
            break;

        default:
            break;
    }
}

void sparta_process_message_status(uint8_t motor_left, uint8_t motor_right)
{
    if(motor_left != SPARTA_STATUS_NO_MOTOR_CHANGE) { /* TODO */ }
    if(motor_right != SPARTA_STATUS_NO_MOTOR_CHANGE) { /* TODO */ }

    uint8_t data[12];
    data[0] = 0xFF; // napięcie baterii, TODO
    data[1] = 0x00; // czujniki linii, TODO
    data[2] = 0x00; // LEDy, TODO
    data[3] = 0xFF; // napięcie lewego silnika, TODO
    data[4] = 0xFF; // napięcie prawego silnika, TODO
    data[5] = 0x00; // sonar lewy, TODO
    data[6] = 0x00; // sonar prawy, TODO
    data[7] = 0xFF; // ADC0 >> 2, TODO
    data[8] = 0xFF; // ADC1 >> 2, TODO
    data[9] = 0xFF; // ADC2 >> 2, TODO
    data[10] = 0x00; // prędkość lewego silnika (-100% => 0, 0% => 100, 100% => 200), TODO
    data[11] = 0x00; // prędkość prawego silnika (-100% => 0, 0% => 100, 100% => 200), TODO

    sparta_send_status(CMD_STAN_CZUJNIKI, data);
}

void sparta_send(SpartaCommand command, uint8_t param)
{
    uart_write(SPARTA_HEADER);
    uart_write(ROBOT_ID);
    uart_write(command);
    uart_write(param);
}

void sparta_send_status(SpartaCommand command, uint8_t data[12])
{
    uart_write(SPARTA_HEADER_STATUS);
    uart_write(ROBOT_ID);
    uart_write(command);
    unsigned char checksum = 0;
    for(unsigned char i = 0; i < 12; i++)
    {
        checksum += data[i];
        uart_write(data[i]);
    }
    uart_write(checksum);
}