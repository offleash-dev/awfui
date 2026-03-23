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

        
        AFVector() : size_(0) {
            // Zero out the buffer explicitly
            memset(buffer_, 0, sizeof(buffer_));
        }
        

        // Add elements one by one
        void add(const T& value) {
            if (size_ < CAPACITY) {
                get_buffer()[size_++] = value;
            }
        }
        

        void push_back(const T& value) {
            if (size_ < CAPACITY) {
                get_buffer()[size_++] = value;
            }
        }
        

        void pop_back() {
            if (size_ > 0) {
                --size_;
            }
        }
        

        T& operator[](size_t index) {
            return get_buffer()[index];
        }
        

        const T& operator[](size_t index) const {
            return get_buffer()[index];
        }
        

        T& back() {
            return get_buffer()[size_ - 1];
        }
        

        const T& back() const {
            return get_buffer()[size_ - 1];
        }
        

        T& front() {
            return get_buffer()[0];
        }
        

        const T& front() const {
            return get_buffer()[0];
        }
        

        size_t size() const { return size_; }
        constexpr size_t capacity() const { return CAPACITY; }
        constexpr size_t max_size() const { return CAPACITY; }
        bool empty() const { return size_ == 0; }
        bool full() const { return size_ >= CAPACITY; }
        
        void clear() { size_ = 0; }
       
        T* data() { return get_buffer(); }
        const T* data() const { return get_buffer(); }
        

        // Iterator support
        class iterator {
        public:
            iterator(T* ptr) : ptr_(ptr) {}
            
            iterator& operator++() { ++ptr_; return *this; }
            iterator operator++(int) { iterator tmp = *this; ++ptr_; return tmp; }
            
            iterator operator+(size_t offset) { return iterator(ptr_ + offset); }
            
            T& operator*() { return *ptr_; }
            const T& operator*() const { return *ptr_; }
            
            bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
            bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
            bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }
            
            friend class AFVector;  // Allow AFVector to access ptr_

            
        private:
            T* ptr_;
        };
        
        iterator begin() { return iterator(get_buffer()); }
        iterator end() { return iterator(get_buffer() + size_); }
        
        // Const iterator
        class const_iterator {
        public:
            const_iterator(const T* ptr) : ptr_(ptr) {}
            
            const_iterator& operator++() { ++ptr_; return *this; }
            const_iterator operator++(int) { const_iterator tmp = *this; ++ptr_; return tmp; }
            
            const T& operator*() const { return *ptr_; }
            
            bool operator!=(const const_iterator& other) const { return ptr_ != other.ptr_; }
            bool operator==(const const_iterator& other) const { return ptr_ == other.ptr_; }

            
        private:
            const T* ptr_;
        };
        

        const_iterator begin() const { return const_iterator(get_buffer()); }
        const_iterator end() const { return const_iterator(get_buffer() + size_); }
        

        // Remove element at iterator position
        iterator erase(iterator pos) {
            if (pos >= end()) return end();
            
            size_t index = pos.ptr_ - get_buffer();
            for (size_t i = index; i < size_ - 1; ++i) {
                get_buffer()[i] = get_buffer()[i + 1];
            }
            --size_;
            return iterator(get_buffer() + index);
        }

        
    private:
        // Manually aligned buffer - no STL dependencies
        alignas(T) char buffer_[sizeof(T) * N];
        size_t size_;
        
        // Helper to get typed pointer to buffer
        T* get_buffer() { return reinterpret_cast<T*>(buffer_); }
        const T* get_buffer() const { return reinterpret_cast<const T*>(buffer_); }
    };
