#pragma once

/**
 * \class CQueue
 * \brief a FIFO queue
 */
template<typename T, int SIZE> class CQueue
{
public:
    CQueue()
    {
        m_head = 0;
        m_tail = 0;
        m_total = 0;
    }

    //! Adds an element to the queue
    void Add(const T &x)
    {
        if (m_total >= SIZE) return;

        m_fifo[m_head++] = x;
        if (m_head >= SIZE)  m_head = 0;
        m_total ++;
    }

    //! Removes and returns an element from queue front
    /** If the queue is empty, returns \c false.
    Else, gets the element from the front, puts it into \a x and returns \c true. */
    bool Get(T &x)
    {
        if (m_head == m_tail)  return false;

        x = m_fifo[m_tail++];
        if (m_tail >= SIZE)  m_tail = 0;
        m_total --;

        return true;
    }

protected:
    volatile T m_fifo[SIZE];
    volatile int m_head;
    volatile int m_tail;
    volatile int m_total;
};
