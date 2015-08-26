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

    DECLARE_CLASS_ISR(USART_RXC_vect);

    //! Waits for previous transmission to finish
    void WaitUntilSendingFinished();
    //! Waits for previous transmission to finish and then sends one byte via UART
    void Send(uint8_t byte);
    //! Sends a null-terminated string
    void SendString(const char* string);

protected:
    //! Called after a byte is recieved
    virtual void ByteRecieved(uint8_t byte) = 0;
};

/**
 * \class CQueuedUart
 * \brief Synchronous UART communication with a recieving queue
 */
class CQueuedUart : public CUart
{
public:
    //! Waits for a byte to be recieved and returns it
    uint8_t Recv();

protected:
    void ByteRecieved(uint8_t byte);

protected:
    static const uint8_t QUEUE_SIZE = 16;
    CQueue<uint8_t, QUEUE_SIZE> m_queue;
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
