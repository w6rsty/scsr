#pragma once

#include "core/type.hpp"
#include <atomic>

namespace scsr
{

/// An lock-free queue based on Michael-Scott algorithm.
/// https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf
template <typename T>
class MSQueue
{
public:
    MSQueue()
    {
        Ref<Node> dumy = make_ref<Node>();
        m_Head.store(dumy);
        m_Tail.store(dumy);
    }

    ~MSQueue() = default;

    void Enqueue(T val)
    {
        Ref<Node> newNode = make_ref<Node>(val);
        Ref<Node> oldTail;

        while (true)
        {
            oldTail = m_Tail.load();
            Ref<Node> next = oldTail->next.load();

            if (oldTail == m_Tail.load())
            {
                if (next == nullptr)
                {
                    if (oldTail->next.compare_exchange_strong(next, newNode))
                    {
                        m_Tail.compare_exchange_strong(oldTail, newNode);
                        break;
                    }
                }
                else
                {
                    m_Tail.compare_exchange_strong(oldTail, next);
                }
            }
        }

    }

    bool Dequeue(T& elem)
    {
        Ref<Node> oldHead;

        while (true)
        {
            oldHead = m_Head.load();
            auto oldTail = m_Tail.load();
            auto next = oldHead->next.load();

            if (oldHead == m_Head.load())
            {
                if (oldHead == oldTail)
                {
                    if (!next)
                    {
                        return false;
                    }
                    m_Tail.compare_exchange_strong(oldTail, next);
                }
                else
                {
                    elem = next->data;
                    if (m_Head.compare_exchange_strong(oldHead, next))
                    {
                        break;
                    }
                }
            }
        }

        return true;
    }
private:
    struct alignas(64) Node
    {
        T data;
        std::atomic<Ref<Node>> next;

        Node() : data(), next(nullptr) {}
        Node(T val) : data(val), next(nullptr) {}
    };

    MSQueue(const MSQueue&) = delete;
    MSQueue& operator=(const MSQueue&) = delete;

    std::atomic<Ref<Node>> m_Head;
    std::atomic<Ref<Node>> m_Tail;
};

}