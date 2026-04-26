#pragma once

//// AFQueue.h
//// Part of the AWFUI library
////
//// Fixed-size queue using aligned buffer (no STL dependencies)
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stddef.h>
#include <stdint.h>



template<typename T, size_t N>
class AFQueue {
public:
        using value_type = T;
        static constexpr size_t CAPACITY = N;
        

        AFQueue() : m_head(0), m_tail(0), m_count(0) {
            // Zero out the buffer explicitly
            memset(m_buffer, 0, sizeof(m_buffer));
        }
        

        bool push(const T& value) {
            if (m_count >= CAPACITY) {
                return false; // Queue full
            }
            
            get_buffer()[m_tail] = value;
            m_tail = (m_tail + 1) % CAPACITY;
            m_count++;
            return true;
        }
        

        bool pop(T& value) {
            if (m_count == 0) {
                return false; // Queue empty
            }
            
            value = get_buffer()[m_head];
            m_head = (m_head + 1) % CAPACITY;
            m_count--;
            return true;
        }

        
        T& front() {
            return get_buffer()[m_head];
        }
        

        const T& front() const {
            return get_buffer()[m_head];
        }
        

        T& back() {
            size_t index = (m_tail == 0) ? CAPACITY - 1 : m_tail - 1;
            return get_buffer()[index];
        }
        

        const T& back() const {
            size_t index = (m_tail == 0) ? CAPACITY - 1 : m_tail - 1;
            return get_buffer()[index];
        }
        

        size_t size() const { return m_count; }
        constexpr size_t capacity() const { return CAPACITY; }
        bool empty() const { return m_count == 0; }
        bool full() const { return m_count >= CAPACITY; }
        

        void clear() {
            m_head = 0;
            m_tail = 0;
            m_count = 0;
        }
        

        // ETL compatibility - pop without parameter
        void pop() {
            if (m_count > 0) {
                m_head = (m_head + 1) % CAPACITY;
                m_count--;
            }
        }
        
        
private:
        // Manually aligned buffer - no STL dependencies
        alignas(T) char m_buffer[sizeof(T) * N];
        size_t m_head;
        size_t m_tail;
        size_t m_count;
        
        // Helper to get typed pointer to buffer
        T* get_buffer() { return reinterpret_cast<T*>(m_buffer); }
        const T* get_buffer() const { return reinterpret_cast<const T*>(m_buffer); }
    };
