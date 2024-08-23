#pragma once

#include <atomic>
#include <optional>

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
        Node* dumy = new Node();
        m_Head.store(dumy);
        m_Tail.store(dumy);
    }

    ~MSQueue()
    {
        while (Node* node = m_Head.load())
        {
            m_Head.store(node->next.load());
            delete node;
        }
    }

    void Enqueue(T val)
    {
        Node* newNode = new Node(val);
        Node* oldTail;

        while (true)
        {
            oldTail = m_Tail.load();
            Node* next = oldTail->next.load();

            if (oldTail == m_Tail.load())
            {
                if (next == nullptr)
                {
                    if (oldTail->next.compare_exchange_weak(next, newNode))
                    {
                        break;
                    }
                }
                else
                {
                    m_Tail.compare_exchange_weak(oldTail, next);
                }
            }
        }

        m_Tail.compare_exchange_weak(oldTail, newNode);
    }

    std::optional<T> Dequeue()
    {
        Node* oldHead;
        T val;

        while (true)
        {
            oldHead = m_Head.load();
            Node* oldTail = m_Tail.load();
            Node* next = oldHead->next.load();

            if (oldHead == m_Head.load())
            {
                if (oldHead == oldTail)
                {
                    if (next == nullptr)
                    {
                        return std::nullopt;
                    }
                    m_Tail.compare_exchange_weak(oldTail, next);
                }
                else
                {
                    val = next->data;
                    if (m_Head.compare_exchange_weak(oldHead, next))
                    {
                        break;
                    }
                }
            }
        }

        delete oldHead;
        return std::make_optional(val);
    }
private:
    struct Node
    {
        T data;
        std::atomic<Node*> next;

        Node() : next(nullptr) {}
        Node(T val) : data(val), next(nullptr) {}
    };
    
    std::atomic<Node*> m_Head;
    std::atomic<Node*> m_Tail;
};

}