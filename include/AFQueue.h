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
        

        AFQueue() : head_(0), tail_(0), count_(0) {
            // Zero out the buffer explicitly
            memset(buffer_, 0, sizeof(buffer_));
        }
        

        bool push(const T& value) {
            if (count_ >= CAPACITY) {
                return false; // Queue full
            }
            
            get_buffer()[tail_] = value;
            tail_ = (tail_ + 1) % CAPACITY;
            count_++;
            return true;
        }
        

        bool pop(T& value) {
            if (count_ == 0) {
                return false; // Queue empty
            }
            
            value = get_buffer()[head_];
            head_ = (head_ + 1) % CAPACITY;
            count_--;
            return true;
        }

        
        T& front() {
            return get_buffer()[head_];
        }
        

        const T& front() const {
            return get_buffer()[head_];
        }
        

        T& back() {
            size_t index = (tail_ == 0) ? CAPACITY - 1 : tail_ - 1;
            return get_buffer()[index];
        }
        

        const T& back() const {
            size_t index = (tail_ == 0) ? CAPACITY - 1 : tail_ - 1;
            return get_buffer()[index];
        }
        

        size_t size() const { return count_; }
        constexpr size_t capacity() const { return CAPACITY; }
        bool empty() const { return count_ == 0; }
        bool full() const { return count_ >= CAPACITY; }
        

        void clear() {
            head_ = 0;
            tail_ = 0;
            count_ = 0;
        }
        

        // ETL compatibility - pop without parameter
        void pop() {
            if (count_ > 0) {
                head_ = (head_ + 1) % CAPACITY;
                count_--;
            }
        }
        
        
    private:
        // Manually aligned buffer - no STL dependencies
        alignas(T) char buffer_[sizeof(T) * N];
        size_t head_;
        size_t tail_;
        size_t count_;
        
        // Helper to get typed pointer to buffer
        T* get_buffer() { return reinterpret_cast<T*>(buffer_); }
        const T* get_buffer() const { return reinterpret_cast<const T*>(buffer_); }
    };
