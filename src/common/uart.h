#pragma once

#include "common/singleton.h"
#include "common/class_isr.h"
#include "common/stdlib/queue.h"

/**
 * \class CUart
 * \brief Base class implementing interrupt-driven UART communication
 */
class CUart : public CSingleton<CUart>
{
public:
    CUart();
    ~CUart();

    CUart(const CUart&) = delete;
    CUart& operator=(const CUart&) = delete;

    DECLARE_CLASS_ISR(USART_RXC_vect);

    //! Waits for previous transmission to finish and then sends one byte via UART
    void Send(uint8_t byte);
    //! Sends a null-terminated string
    void SendString(const char* string);

protected:
    //! Called after a byte is recieved
    virtual void ByteRecieved(uint8_t byte) = 0;
};

/**
 * \class CAsyncUart
 * \brief Asynchronous UART communication, calls a function every time a byte is received
 */
class CAsyncUart : public CUart
{
public:
    typedef void(*RecvCallbackFunc)(uint8_t);
    CAsyncUart(RecvCallbackFunc recvCallback);

protected:
    void ByteRecieved(uint8_t byte);

protected:
    RecvCallbackFunc m_callback;
};
