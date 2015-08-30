#pragma once

#include <stdint.h>

/**
 * \class CBuffer
 * \brief a buffer
 */
template<typename T, uint8_t MAX_SIZE> class CBuffer
{
public:
    CBuffer()
    {
        Flush();
    }

    //! Adds an element to the end of the buffer
    void Add(const T &x)
    {
        if (m_size >= MAX_SIZE) return;
        m_buffer[m_size++] = x;
    }

    //! Returns the current size of the buffer
    uint8_t Size()
    {
        return m_size;
    }

    //! Returns pointer to the start of the buffer
    const T* Get()
    {
        return &m_buffer;
    }

    //! Resets the buffer
    void Flush()
    {
        m_size = 0;
    }

    //! Returns element of the buffer
    T operator[](uint8_t index) {
        return m_buffer[index];
    }

private:
    volatile T m_buffer[MAX_SIZE];
    volatile uint8_t m_size;
};
