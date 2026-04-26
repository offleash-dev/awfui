#pragma once

//// AFVector.h
//// Part of the AWFUI library
////
//// Fixed-size vector using aligned buffer (no STL dependencies)
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stddef.h>
#include <stdint.h>
#include <string.h> 



template<typename T, size_t N>
class AFVector {
    public:
        using value_type = T;
        static constexpr size_t CAPACITY = N;

        
        AFVector() : m_size(0) {
            // Zero out the buffer explicitly
            memset(m_buffer, 0, sizeof(m_buffer));
        }
        

        // Add elements one by one
        void add(const T& value) {
            if (m_size < CAPACITY) {
                get_buffer()[m_size++] = value;
            }
        }
        

        void push_back(const T& value) {
            if (m_size < CAPACITY) {
                get_buffer()[m_size++] = value;
            }
        }
        

        void pop_back() {
            if (m_size > 0) {
                --m_size;
            }
        }
        

        T& operator[](size_t index) {
            return get_buffer()[index];
        }
        

        const T& operator[](size_t index) const {
            return get_buffer()[index];
        }
        

        T& back() {
            return get_buffer()[m_size - 1];
        }
        

        const T& back() const {
            return get_buffer()[m_size - 1];
        }
        

        T& front() {
            return get_buffer()[0];
        }
        

        const T& front() const {
            return get_buffer()[0];
        }
        


        size_t size() const { return m_size; }
        constexpr size_t capacity() const { return CAPACITY; }
        constexpr size_t max_size() const { return CAPACITY; }
        bool empty() const { return m_size == 0; }
        bool full() const { return m_size >= CAPACITY; }
        
        void clear() { m_size = 0; }
       
        T* data() { return get_buffer(); }
        const T* data() const { return get_buffer(); }
        


        // Iterator support
        class iterator {
        public:
            iterator(T* ptr) : m_ptr(ptr) {}
            
            iterator& operator++() { ++m_ptr; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++m_ptr; return tmp; }
            
            iterator operator+(size_t offset) { return iterator(m_ptr + offset); }
            
            T& operator*() { return *m_ptr; }
            const T& operator*() const { return *m_ptr; }
            
            bool operator!=(const iterator& other) const { return m_ptr != other.m_ptr; }
            bool operator==(const iterator& other) const { return m_ptr == other.m_ptr; }
            bool operator>=(const iterator& other) const { return m_ptr >= other.m_ptr; }
            
            friend class AFVector;  // Allow AFVector to access m_ptr

            
        private:
            T* m_ptr;
        };
        

        iterator begin() { return iterator(get_buffer()); }
        iterator end() { return iterator(get_buffer() + m_size); }
        

        // Const iterator
        class const_iterator {
        public:
            const_iterator(const T* ptr) : m_ptr(ptr) {}
            
            const_iterator& operator++() { ++m_ptr; return *this; }
            const_iterator operator++(int) { const_iterator tmp = *this; ++m_ptr; return tmp; }
            
            const T& operator*() const { return *m_ptr; }
            
            bool operator!=(const const_iterator& other) const { return m_ptr != other.m_ptr; }
            bool operator==(const const_iterator& other) const { return m_ptr == other.m_ptr; }

            
        private:
            const T* m_ptr;
        };
        


        const_iterator begin() const { return const_iterator(get_buffer()); }
        const_iterator end() const { return const_iterator(get_buffer() + m_size); }
        

        
        // Remove element at iterator position
        iterator erase(iterator pos) {
            if (pos >= end()) return end();
            
            size_t index = pos.m_ptr - get_buffer();
            for (size_t i = index; i < m_size - 1; ++i) {
                get_buffer()[i] = get_buffer()[i + 1];
            }
            --m_size;
            return iterator(get_buffer() + index);
        }

        
    private:
        // Manually aligned buffer - no STL dependencies
        alignas(T) char m_buffer[sizeof(T) * N];
        size_t m_size;
        
        // Helper to get typed pointer to buffer
        T* get_buffer() { return reinterpret_cast<T*>(m_buffer); }
        const T* get_buffer() const { return reinterpret_cast<const T*>(m_buffer); }
    };
