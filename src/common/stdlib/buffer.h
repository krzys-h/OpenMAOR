#pragma once

#include "common/stdlib/assert.h"

/**
 * \class CBuffer
 * \brief a buffer
 */
template<typename T, int MAX_SIZE> class CBuffer
{
public:
    CBuffer()
    {
        Flush();
    }

    //! Adds an element to the end of the buffer
    void Add(const T &x)
    {
        assert(m_size < MAX_SIZE);
        m_buffer[m_size++] = x;
    }

    //! Returns the current size of the buffer
    int Size()
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
    T operator[](int index) {
        return m_buffer[index];
    }

protected:
    volatile T m_buffer[MAX_SIZE];
    volatile int m_size;
};