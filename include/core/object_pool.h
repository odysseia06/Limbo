#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <cassert>
#include <cstring>
#include <iostream>

/**
 * A fixed-capacity (non-expanding) ObjectPool for type T.
 *
 * Usage:
 *   1) Construct ObjectPool<T>(capacity).
 *   2) Call Allocate(...) to construct a T in-place and get a std::shared_ptr<T>.
 *   3) When that shared_ptr dies, the custom deleter calls the destructor and
 *      returns the slot to the pool.
 *
 * If the pool is full (no free slots), Allocate(...) returns nullptr.
 */
template <typename T>
class ObjectPool
{
public:
    /**
     * Constructor
     * @param capacity  Number of T objects this pool can hold at once.
     * @param zeroInit  If true, the storage bytes are zero-initialized at creation.
     */
    ObjectPool(size_t capacity, bool zeroInit = false)
        : m_Capacity(capacity),
        m_ActiveCount(0),
        m_PeakUsage(0)
    {
        // Allocate raw byte storage
        size_t totalBytes = m_Capacity * sizeof(T);
        m_Storage = std::make_unique<unsigned char[]>(totalBytes);

        // Optionally zero-init the storage
        if (zeroInit)
        {
            std::memset(m_Storage.get(), 0, totalBytes);
        }

        // Fill free list with all indices [0..capacity-1]
        m_FreeList.reserve(m_Capacity);
        for (size_t i = 0; i < m_Capacity; ++i)
        {
            m_FreeList.push_back(i);
        }
    }

    // Non-copyable (you can make this copyable if you wish to design that carefully)
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    /**
     * Allocate an object of type T in the pool using placement new.
     * Returns a std::shared_ptr<T>.
     *
     * If the pool is full, returns nullptr.
     */
    template <typename... Args>
    std::shared_ptr<T> Allocate(Args&&... args)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        // If no free slots, pool is full
        if (m_FreeList.empty())
        {
            return nullptr; // or throw, depending on your design
        }

        // Take the last free index
        size_t index = m_FreeList.back();
        m_FreeList.pop_back();

        // Calculate the correct address in our raw storage
        unsigned char* slotPtr = &m_Storage[index * sizeof(T)];
        // Construct the object in-place with placement new
        T* objPtr = new (slotPtr) T(std::forward<Args>(args)...);

        // Create a custom deleter that:
        //  1) calls the destructor,
        //  2) returns the slot to m_FreeList,
        //  3) decrements m_ActiveCount
        auto deleter = [this, index](T* ptr) {
            std::lock_guard<std::mutex> dlock(m_Mutex);
            // Explicitly call the destructor
            ptr->~T();
            // Return the slot to the free list
            m_FreeList.push_back(index);
            // Decrement active count
            --m_ActiveCount;
            };

        // Update usage counts
        ++m_ActiveCount;
        if (m_ActiveCount > m_PeakUsage)
        {
            m_PeakUsage = m_ActiveCount;
        }

        // Return a shared_ptr that uses our custom deleter
        return std::shared_ptr<T>(objPtr, deleter);
    }

    /**
     * Returns how many objects are currently allocated (i.e., active).
     */
    size_t GetActiveCount() const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_ActiveCount;
    }

    /**
     * Returns the maximum number of objects that have been active simultaneously.
     */
    size_t GetPeakUsage() const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_PeakUsage;
    }

    /**
     * Returns the total capacity (number of T objects this pool can hold).
     */
    size_t GetCapacity() const
    {
        return m_Capacity;
    }

private:
    mutable std::mutex m_Mutex;  // Protects access to free list, counts

    // Raw storage for all T objects
    std::unique_ptr<unsigned char[]> m_Storage;
    const size_t m_Capacity;      // Fixed number of T we can hold

    std::vector<size_t> m_FreeList; // Indices of unused slots

    // Tracking usage
    size_t m_ActiveCount; // how many objects are currently in use
    size_t m_PeakUsage;   // the highest m_ActiveCount has ever been
};
